/**
 * @file Device.cpp
 * @author B站 @沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 本文件包含了对项目硬件的具体驱动实现，包括：
 * - WS2812 彩灯的初始化与控制。
 * - 物理按键的事件处理，支持单击、长按和双击。
 * - 电源管理，包括电池电压读取和充电状态检测。
 */

#include "Device.h"

// 外部全局充电事件标志
bool g_charging_started_event = false;

/******************************************************************************
 *                            彩灯驱动 (WS2812 Driver)
 ******************************************************************************/

/**
 * @brief 声明并初始化 SYC_WS2812 驱动对象。
 * @param ws2812_number LED灯珠的数量。
 * @param ws2812_brightness 初始亮度 (0-255)。
 */
SYC_WS2812 strip = SYC_WS2812(ws2812_number, ws2812_brightness);

/**
 * @brief 初始化WS2812 LED灯条。
 * @details 此函数应在 setup() 函数中调用，以准备LED灯条的GPIO并开始通信。
 */
void WS2812_Init()
{
  strip.setup();
}
/***************************************************************************/

/******************************************************************************
 *                            按键驱动 (Key Driver)
 ******************************************************************************/

// 记录按键被按下的起始时间
static unsigned long left_key_down_time = 0;
static unsigned long right_key_down_time = 0;

// 标记长按事件是否已经触发过，以防止在一次长按中重复触发事件
static bool left_key_long_press_fired = false;
static bool right_key_long_press_fired = false;

/**
 * @brief 初始化按键所需的GPIO引脚。
 * @details 将按键引脚设置为上拉输入模式。当按键按下时，引脚电平为低。
 */
void Key_Init() {
    pinMode(LEFT_KEY_PIN, INPUT_PULLUP);
    pinMode(RIGHT_KEY_PIN, INPUT_PULLUP);
}

/**
 * @brief 读取并返回当前的按键事件。
 * @details 这是一个非阻塞的状态机函数，可在主循环中被频繁调用。
 *          它能有效地处理按键抖动，并识别单击、长按和双键齐按事件。
 * @return KeyEvent枚举，表示当前检测到的按键事件。如果没有事件，则返回 NO_EVENT。
 */
KeyEvent read_key_event() {
    // 读取当前按键的物理状态 (低电平为按下)
    bool left_key_pressed = !digitalRead(LEFT_KEY_PIN);
    bool right_key_pressed = !digitalRead(RIGHT_KEY_PIN);

    // ----- 1. 优先处理双键同时按下的情况 -----
    // 当检测到两个键都按下时，立即处理并返回，以防止被后续逻辑误判为单击或长按。
    if (left_key_pressed && right_key_pressed) {
        // 消抖
        delay(DEBOUNCE_TIME);
        // 再次确认是否双键按下
        if (!digitalRead(LEFT_KEY_PIN) && !digitalRead(RIGHT_KEY_PIN)) {
            // 等待按键被释放。
            while(!digitalRead(LEFT_KEY_PIN) && !digitalRead(RIGHT_KEY_PIN)) {
                yield(); // 继续处理其他任务
            }
            return KeyEvent::BOTH_PRESS; // 双键按下
        }
    }

    // ----- 2. 处理左键 -----
    if (left_key_pressed) {
        // 如果 left_key_down_time 为0，说明按键刚被按下
        if (left_key_down_time == 0) {
            left_key_down_time = millis();     // 记录按键按下的时间戳
            left_key_long_press_fired = false; // 重置长按触发标志
        } else if (!left_key_long_press_fired && (millis() - left_key_down_time > LONG_PRESS_TIME)) {
            // 如果按键持续按下超过了设定的长按时间，并且长按事件还未触发过
            left_key_long_press_fired = true;  // 长按触发
            return KeyEvent::LEFT_LONG_PRESS;    // 左键长按
        }
    } else { // 左键释放
        // 如果 left_key_down_time 大于0，说明按键刚被释放
        if (left_key_down_time > 0) {
            // 检查按下持续时间：必须大于消抖时间，且长按事件未被触发
            if (!left_key_long_press_fired && (millis() - left_key_down_time > DEBOUNCE_TIME)) {
                left_key_down_time = 0;      // 重置时间
                return KeyEvent::LEFT_CLICK; // 左键单击
            }
            // 长按释放
            left_key_down_time = 0;
        }
    }

    // ----- 3. 处理右键（逻辑与左键相同） -----
    if (right_key_pressed) {
        if (right_key_down_time == 0) {
            right_key_down_time = millis();
            right_key_long_press_fired = false;
        } else if (!right_key_long_press_fired && (millis() - right_key_down_time > LONG_PRESS_TIME)) {
            right_key_long_press_fired = true;
            return KeyEvent::RIGHT_LONG_PRESS;
        }
    } else {
        if (right_key_down_time > 0) {
            if (!right_key_long_press_fired && (millis() - right_key_down_time > DEBOUNCE_TIME)) {
                right_key_down_time = 0;
                return KeyEvent::RIGHT_CLICK;
            }
            right_key_down_time = 0;
        }
    }

    // 无按键按下
    return KeyEvent::NO_EVENT;
}
/***************************************************************************/

/******************************************************************************
 *                         电源管理 (Power Management)
 ******************************************************************************/
// 保存当前电池电量等级
static BatteryLevel g_current_level = LEVEL_FULL;
// 保存当前充电状态
static ChargingState g_charging_state = STATE_DISCHARGING;
// 记录上次检查电压的时间
static unsigned long g_last_voltage_check_time = 0;

/**
 * @brief 初始化电源管理模块相关的引脚
 */
void Voltage_Init() {
    // 设置 ADC_PIN 为模拟输入模式，用于读取电池电压
    pinMode(ADC_PIN, INPUT);
    // 设置 CHRG_PIN 为上拉输入模式
    pinMode(CHRG_PIN, INPUT_PULLUP);
    // 设置ADC的分辨率为12位
    analogReadResolution(12);
    // 程序启动时立即获取一次电压状态
    updateVoltageState();
}

/**
 * @brief 更新当前的电池电量等级。
 * @details 此函数读取电池电压，并根据预设的电压阈值将其映射到相应的电量等级。
 */
void updateVoltageState() {
    uint16_t voltage = readBatteryVoltage();
    if (voltage >= VOLTAGE_LEVEL_FULL) {
        g_current_level = LEVEL_FULL;
    } else if (voltage >= VOLTAGE_LEVEL_HIGH) {
        g_current_level = LEVEL_HIGH;
    } else if (voltage >= VOLTAGE_LEVEL_MEDIUM) {
        g_current_level = LEVEL_MEDIUM;
    } else if (voltage >= VOLTAGE_LEVEL_LOW) {
        g_current_level = LEVEL_LOW;
    } else {
        g_current_level = LEVEL_EMPTY;
    }
}


/**
 * @brief 更新当前的充电状态。
 */
void updateChargingState() {
    // 读取充电状态引脚的状态
    bool is_currently_charging = (digitalRead(CHRG_PIN) == LOW);

    // 状态切换检测：从"非充电状态" 变为 "充电状态"
    if (is_currently_charging && g_charging_state != STATE_CHARGING && g_charging_state != STATE_CHARGE_FULL) {
        // 这是唯一的事件触发点：检测到充电开始的"上升沿"
        g_charging_started_event = true;

        // 更新当前状态
        g_charging_state = STATE_CHARGING;
    }
    // 状态切换检测：正在充电中，检测是否充满
    else if (is_currently_charging && g_charging_state == STATE_CHARGING) {
        // 当电压达到满电阈值时，判定为充满
        uint16_t voltage = readBatteryVoltage();
        if (voltage >= VOLTAGE_LEVEL_FULL) {
            g_charging_state = STATE_CHARGE_FULL;
            g_charging_started_event = true;  // 触发充满事件
        }
    }
    // 状态切换检测：从"充电状态/充满状态" 变为 "非充电状态"
    else if (!is_currently_charging && (g_charging_state == STATE_CHARGING || g_charging_state == STATE_CHARGE_FULL)) {
        g_charging_state = STATE_DISCHARGING;
    }
}


/**
 * @brief 获取当前的电池电量等级。
 * @return BatteryLevel 枚举值。
 */
BatteryLevel getCurrentBatteryLevel() {
    return g_current_level;
}

/**
 * @brief 获取当前的充电状态。
 * @return ChargingState 枚举值。
 */
ChargingState getCurrentChargingState() {
    return g_charging_state;
}

/**
 * @brief 读取并返回经过分压电路校正后的实时电池电压。
 * @return 电池电压，单位为毫伏 (mV)。
 */
uint16_t readBatteryVoltage() {
    // 使用 analogReadMilliVolts() 获取ADC引脚上的电压(mV)
    uint32_t battery_voltage_mv = analogReadMillivolts(ADC_PIN) * 2;

    return (uint16_t)battery_voltage_mv;
}

/**
 * @brief 电源管理任务函数。
 */
void Voltage_task()
{
    static unsigned long last_Voltage_Check = 0;
    static unsigned long last_Charging_Check = 0;

    // 每隔 VOLTAGE_CHECK_INTERVAL 毫秒检查一次电池电压
    if (millis() - last_Voltage_Check > VOLTAGE_CHECK_INTERVAL) {
        last_Voltage_Check = millis();
        updateVoltageState();
    }
    // 每隔 CHARGING_CHECK_INTERVAL 毫秒检查一次充电状态
    if (millis() - last_Charging_Check > CHARGING_CHECK_INTERVAL) {
        last_Charging_Check = millis();
        updateChargingState();
    }
}
/***************************************************************************/