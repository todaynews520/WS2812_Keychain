/**
 * @file Device.h
 * @author B站 沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 此文件定义了所有硬件驱动相关的配置、引脚分配、全局变量声明和函数原型。
 * 它是底层硬件抽象层，为上层应用提供统一的接口。
 */

#ifndef _DEVICE_H_
#define _DEVICE_H_

// ------------------- 核心及库文件包含 -------------------
#include <Arduino.h>         
#include "WS2812_SYC_Air001.h" 
#include "enums.h"       
#include <pgmspace.h>    
#include "Bitmap.h"      
#include <EEPROM.h>      

/**
 * @brief 使用 extern 声明全局唯一的WS2812驱动对象。
 */
extern SYC_WS2812 strip;

/******************************************************************************
 *                          彩灯配置 (WS2812 Configuration)
 ******************************************************************************/

/**
 * @brief LED灯珠的总数量。
 */
const int ws2812_number = 64;

/**
 * @brief WS2812的初始亮度。
 */
const int ws2812_brightness = 80;

/**
 * @brief 初始化WS2812 LED灯条。
 */
void WS2812_Init();
/***************************************************************************/


/******************************************************************************
 *                          按键配置 (Key Configuration)
 ******************************************************************************/

// --- 按键引脚定义 ---
/**
 * @brief 左键连接的GPIO引脚。
 */
const int LEFT_KEY_PIN  = PA4;

/**
 * @brief 右键连接的GPIO引脚。
 */
const int RIGHT_KEY_PIN = PA1;

// --- 按键时序参数配置 ---
/**
 * @brief 按键消抖时间 (单位: 毫秒)。在此时间内的电平变化被视作抖动。
 */
const unsigned long DEBOUNCE_TIME = 20;

/**
 * @brief 判定为长按所需的最短持续时间 (单位: 毫秒)。
 */
const unsigned long LONG_PRESS_TIME = 800;

// --- 按键驱动函数声明 ---
/**
 * @brief 初始化按键所需的GPIO引脚。
 */
void Key_Init(void);

/**
 * @brief 读取并返回当前的按键事件。
 * @return KeyEvent 枚举值，表示检测到的事件（如单击、长按等）。
 */
KeyEvent read_key_event(void);
/***************************************************************************/


/******************************************************************************
 *                           电源管理配置 (Power Management)
 ******************************************************************************/

/**
 * @brief 外部全局充电事件标志。
 */
extern bool g_charging_started_event;

// --- 电源相关引脚定义 ---
/**
 * @brief 连接到电池电压采样分压电路的ADC引脚。
 */
const uint32_t ADC_PIN  = PA6;
/**
 * @brief 连接到充电管理IC的状态指示引脚 (通常是 CHRG 或 STAT)。
 */
const int CHRG_PIN = PA0;

// --- ADC 和分压电路参数 ---
/**
 * @brief ADC的参考电压 (单位: V)。
 */
const float ADC_VREF = 3.3f;
/**
 * @brief 电池电压采样分压电阻R1的阻值 (单位: Ohm)。
 */
const uint32_t R1_VALUE = 10000;
/**
 * @brief 电池电压采样分压电阻R2的阻值 (单位: Ohm)。
 */
const uint32_t R2_VALUE = 10000;

// --- 电量等级电压阈值 (单位: 毫伏 mV) ---
/**
 * @brief 判定为“满电”的电压阈值。
 */
const uint16_t VOLTAGE_LEVEL_FULL = 4000;   // 4.00V
/**
 * @brief 判定为“高电量”的电压阈值。
 */
const uint16_t VOLTAGE_LEVEL_HIGH = 3850;   // 3.85V
/**
 * @brief 判定为“中等电量”的电压阈值。
 */
const uint16_t VOLTAGE_LEVEL_MEDIUM = 3700; // 3.70V
/**
 * @brief 判定为“低电量”的电压阈值。
 */
const uint16_t VOLTAGE_LEVEL_LOW = 3550;    // 3.55V

// --- 状态更新间隔 (单位: 毫秒 ms) ---
/**
 * @brief 检查充电状态的频率。
 */
const unsigned long CHARGING_CHECK_INTERVAL = 200;
/**
 * @brief 检查电池电压的频率。
 */
const unsigned long VOLTAGE_CHECK_INTERVAL = 5000;


// --- 电源管理函数声明 ---

/**
 * @brief 初始化电源管理模块相关的引脚。
 */
void Voltage_Init(void);

/**
 * @brief 更新当前的电池电量等级 (基于电压阈值)。
 */
void updateVoltageState(void);

/**
 * @brief 更新当前的充电状态 (检测充电IC的状态引脚)。
 */
void updateChargingState(void);

/**
 * @brief 获取当前的电池电量等级。
 * @return BatteryLevel 枚举值，表示当前电量。
 */
BatteryLevel getCurrentBatteryLevel(void);

/**
 * @brief 获取当前的充电状态。
 * @return ChargingState 枚举值，表示充电状态。
 */
ChargingState getCurrentChargingState(void);

/**
 * @brief 读取并返回校正后的实时电池电压。
 * @return uint16_t 电池电压值 (单位: 毫伏 mV)。
 */
uint16_t readBatteryVoltage(void);

/**
 * @brief 电源管理的周期性任务函数，应在主循环中调用。
 */
void Voltage_task(void);

// --- UI 显示相关函数 ---

/**
 * @brief 渲染并显示当前的电池电量图标。
 */
void render_battery_display(void);

/**
 * @brief 显示“满电”图标。 
 */
void LEVEL_FULL_icon(SYC_WS2812& ws);

/**
 * @brief 显示“高电量”图标。 
 */
void LEVEL_HIGH_icon(SYC_WS2812& ws);

/**
 * @brief 显示“中等电量”图标。 
 */
void LEVEL_MEDIUM_icon(SYC_WS2812& ws);

/**
 * @brief 显示“低电量”图标。 
 */
void LEVEL_LOW_icon(SYC_WS2812& ws);

/**
 * @brief 显示“电量耗尽”图标的第一帧（用于动画）。 
 */
void LEVEL_EMPTY_icon_1(SYC_WS2812& ws);

/**
 * @brief 显示“电量耗尽”图标的第二帧（用于动画）。 
 */
void LEVEL_EMPTY_icon_2(SYC_WS2812& ws);

/**
 * @brief 显示亮度等级1的图标。 
 */
void LEVEL_BRIGHTNESS_icon_1(SYC_WS2812& ws);

/**
 * @brief 显示亮度等级2的图标。 
 */
void LEVEL_BRIGHTNESS_icon_2(SYC_WS2812& ws);
/**
 * @brief 显示亮度等级3的图标。 
 */
void LEVEL_BRIGHTNESS_icon_3(SYC_WS2812& ws);

/**
 * @brief 显示亮度等级4的图标。 
 */
void LEVEL_BRIGHTNESS_icon_4(SYC_WS2812& ws);

/**
 * @brief 显示亮度等级5的图标。 
 */
void LEVEL_BRIGHTNESS_icon_5(SYC_WS2812& ws);

/***************************************************************************/

#endif