/***************************************************************************************************
 * @file Animation.cpp
 * @author B站 @沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 本文件包含多种动画效果，包括：
 * - 火焰动画
 * - 流动的彩虹动画
 * - 跳动的心动画
 * - 流星雨动画
 * - LOGO动画
 ***************************************************************************************************/

#include "Animation.h"

/******************************************************************************
 *                        火焰动画 (Flame Animation)
 ******************************************************************************/

/**
 * @brief 将热度值 (0-255) 转换为对应的火焰颜色。
 *
 * 这是火焰效果的辅助函数。它将温度值映射到一个从黑 -> 红 -> 橙 -> 黄 的渐变
 *
 * @param temperature 热度值，0为最冷（黑色），255为最热（亮黄色）。
 * @return 返回一个32位的GRB格式颜色值。
 */
uint32_t heatToColor_lowRam(byte temperature) {
    if (temperature == 0) return 0; // 黑色

    // --- 计算 R, G, B 分量 ---
    uint8_t r, g, b;

    // 黑(0,0,0) -> 深红(180,0,0)
    if (temperature <= 85) {
        r = map(temperature, 0, 85, 0, 180);
        g = 0;
        b = 0;
    // 深红(180,0,0) -> 亮橙(255,100,0)
    } else if (temperature <= 170) {
        r = map(temperature, 86, 170, 180, 255);
        g = map(temperature, 86, 170, 0, 100);
        b = 0;
    // 亮橙(255,100,0) -> 亮黄(255,255,0)
    } else {
        r = 255;
        g = map(temperature, 171, 255, 100, 255);
        b = 0;
    }

    // 组合成32位颜色值
    return ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

/**
 * @brief 在8x8 LED矩阵上生成一个低内存占用的逼真火焰动画。
 *
 * 该效果通过模拟像素冷却、热量向上传播以及在底部随机产生新火花来模拟火焰。
 *
 * @param ws SYC_WS2812驱动对象的引用。
 * @param cooling 冷却值，决定火焰熄灭的速度，值越大，冷却越快(建议值: 20-80)
 * @param sparking 火花值 (0-255)，代表在底部产生新火花的概率，值越大，火花越多(建议值: 50-200)
 * @param reversed 方向，如果为 false，火焰向上燃烧；如果为 true，火焰向下流动
 */
void flameEffect_lowRam(SYC_WS2812& ws, int cooling, int sparking, bool reversed) {
    // 定义画布尺寸
    const int WIDTH = 8;
    const int HEIGHT = 8;
    // 每个像素的热度
    static uint8_t heat[ws2812_number];

    // --- 步骤 1: 冷却画布 ---
    for (int i = 0; i < ws2812_number; i++) {
        int cooldown = random(0, ((cooling * 10) / HEIGHT) + 2);
        heat[i] = (heat[i] > cooldown) ? (heat[i] - cooldown) : 0;
    }

    // --- 步骤 2: 热量扩散 ---
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // 计算当前像素的索引 (行式排布)
            int current_idx;
            if (reversed) {
                 current_idx = (HEIGHT - 1 - y) * WIDTH + x;
            } else {
                 current_idx = y * WIDTH + x;
            }

            // 读取下方三个像素(左下, 正下, 右下)以及更下方一个像素的热量，以产生更自然的火焰飘动效果
            int heat_down_left  = (y < HEIGHT - 1 && x > 0)     ? heat[(y+1)*WIDTH + (x-1)] : 0;
            int heat_down       = (y < HEIGHT - 1)              ? heat[(y+1)*WIDTH + x]     : 0;
            int heat_down_right = (y < HEIGHT - 1 && x < WIDTH-1) ? heat[(y+1)*WIDTH + (x+1)] : 0;
            int heat_further_down = (y < HEIGHT - 2)              ? heat[(y+2)*WIDTH + x]     : 0;

            // 进行加权平均，模拟热空气不均匀地向上传播
            int new_heat = (heat_down * 3 + heat_down_left + heat_down_right + heat_further_down) / 6;

            // 将计算出的新热量写入当前像素
            heat[current_idx] = new_heat;
        }
    }

    // --- 步骤 3: 在底部随机点燃火花 ---
    if (random(255) < sparking) {
        int x = random(1, WIDTH - 2);       // 不在最边缘点火，效果更自然
        int y = reversed ? 0 : HEIGHT - 1;  // 在最下面一行
        int spark_idx = y * WIDTH + x;
        heat[spark_idx] = random(160, 255); // 赋予新火花一个高的初始热度
    }

    // --- 步骤 4: 将热度图映射为颜色并显示 ---
    for (int i = 0; i < ws2812_number; i++) {
        strip.setWs2812Color(i, heatToColor_lowRam(heat[i]));
    }
}

/******************************************************************************
 *                       彩虹动画 (Rainbow Animation)
 ******************************************************************************/

/**
 * @brief 创建流动的彩虹效果。
 *
 * 此函数会生成一个平滑、循环的彩虹图案
 * 移动速度和彩虹的密度都是可调的。
 *
 * @param ws SYC_WS2812驱动对象的引用。
 * @param speed 控制动画的速度，值越大速度越快。
 * @param density 控制彩虹的密度，值越大颜色带越多、越窄。
 */
void anim_rainbow_flow(SYC_WS2812& ws, uint8_t speed, uint8_t density) {
    // 计算时间分量
    uint32_t time_component = millis() / (100 / speed);

    for (int i = 0; i < ws2812_number; i++) {
        byte hue = (i * density + time_component) & 255;
        uint32_t color = ws.Wheel(hue);
        // 使用 setWs2812Color 函数来应用亮度并设置像素颜色
        ws.setWs2812Color(i, color);
    }
}


/******************************************************************************
 *                         跳动的心动画 (Beating Heart)
 ******************************************************************************/

/**
 * @brief 显示一个两帧的跳动心动画。
 *
 * 此函数通过在两个预定义的爱心图像（`heart1`和`heart2`）之间切换，
 * 来创造一个简单的心跳效果。
 *
 * @param ws SYC_WS2812驱动对象的引用。
 * @param interval 每一帧之间的间隔时间（毫秒），用于控制播放速度。
 */
void anim_beating_heart(SYC_WS2812& ws, uint16_t interval) {
    static unsigned long last_frame_time = 0;
    static uint8_t current_frame = 0;

    // 检查是否已达到切换到下一帧的时间
    if (millis() - last_frame_time >= interval) {
        last_frame_time = millis();
        current_frame = (current_frame + 1) % 2; // 在第0帧和第1帧之间切换
    }

    // 根据当前帧数显示对应的图像
    if (current_frame == 0) {
        ws.Draw(heart1_num, heart1);
    } else {
        ws.Draw(heart2_num, heart2);
    }
}

/******************************************************************************
 *                         流星雨动画 (Meteor Shower)
 ******************************************************************************/

bool meteor_animation_initialized = false;
Meteor meteor_pool[MAX_METEORS];

/**
 * @brief 初始化流星雨动画。
 */
void init_meteor_shower() {
    for (int i = 0; i < MAX_METEORS; i++) {
        meteor_pool[i].is_active = false; 
    }
    strip.clearWs2812(); // 清空屏幕
    meteor_animation_initialized = true;
}

/**
 * @brief 渲染一个带拖尾效果的流星雨动画。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param new_meteor_chance 每一帧产生新流星的概率 (0-255)，建议值为 10-40。
 */
void anim_meteor_shower(SYC_WS2812& ws, uint8_t new_meteor_chance) {
    // 如果动画是首次运行，则进行初始化
    if (!meteor_animation_initialized) {
        init_meteor_shower();
    }

    // ---- 1. 绘制拖尾效果 ----
    for (int i = 0; i < ws2812_number; i++) {
        uint32_t color = ws.led_data[i];
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        r = (r <= FADE_RATE) ? 0 : r - FADE_RATE;
        g = (g <= FADE_RATE) ? 0 : g - FADE_RATE;
        b = (b <= FADE_RATE) ? 0 : b - FADE_RATE;

        ws.led_data[i] = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
    }

    // --- 2. 生成一颗新的流星 ---
    if (random(255) < new_meteor_chance) {
        for (int i = 0; i < MAX_METEORS; i++) {
            if (!meteor_pool[i].is_active) {
                meteor_pool[i].is_active = true;
                meteor_pool[i].x = random(8) << 8;
                meteor_pool[i].y = 0;
                meteor_pool[i].speed = random(256, 768);
                // 流星的颜色
                meteor_pool[i].hue = random(0, 256);
                break; // 找到后即退出循环
            }
        }
    }

    // ---- 3. 移动并绘制所有活动的流星 ----
    for (int i = 0; i < MAX_METEORS; i++) {
        if (meteor_pool[i].is_active) {
            meteor_pool[i].y += meteor_pool[i].speed / 4;

            if (meteor_pool[i].y >= (8 << 8)) {
                meteor_pool[i].is_active = false;
            } else {
                int x_pos = meteor_pool[i].x >> 8;
                int y_pos = meteor_pool[i].y >> 8;

                int index = y_pos * 8 + x_pos;
                if (index >= 0 && index < ws2812_number) {
                    ws.led_data[index] = ws.Wheel(meteor_pool[i].hue);
                }
            }
        }
    }
}


/******************************************************************************
 *                           LOGO动画 (Logo Animation)
 ******************************************************************************/

/**
 * @brief 显示一个简单的两帧LOGO动画。
 *
 * 此函数通过在两个预定义的LOGO图像 (`Animation_logo1` 和 `Animation_logo2`) 之间切换，
 * 来创造一个基础的动画效果。
 *
 * @param ws SYC_WS2812驱动对象的引用。
 * @param interval 每一帧之间的间隔时间（毫秒），用于控制动画的速度。
 */
void anim_logo(SYC_WS2812& ws, uint16_t interval) {
    static unsigned long last_frame_time = 0;
    static uint8_t current_frame = 0;

    // 检查是否已达到切换到下一帧的时间
    if (millis() - last_frame_time >= interval) {
        last_frame_time = millis();
        current_frame = (current_frame + 1) % 2; // 在第0帧和第1帧之间切换
    }

    // 根据当前帧数显示对应的图像
    if (current_frame == 0) {
        ws.Draw(Animation_logo1_num, Animation_logo1);
    } else {
        ws.Draw(Animation_logo2_num, Animation_logo2);
    }
}