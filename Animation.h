/**
 * @file Animation.h
 * @author B站 @沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果函数的声明与配置。
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 此头文件定义了所有动画函数的接口、相关配置宏以及所需的外部变量。
 */

#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Device.h" // 引入设备驱动，其中应包含 SYC_WS2812 类和 Meteor 结构体的定义

/******************************************************************************
 *                        动画配置宏与全局变量 (Configurations)
 ******************************************************************************/

// --- 流星雨动画配置 (Meteor Shower Settings) ---

/**
 * @brief 定义屏幕上可以同时存在的最大流星数量。
 * @note 增加此值会轻微增加RAM使用量。
 */
#define MAX_METEORS 5

/**
 * @brief 定义流星拖尾的渐隐速度 (0-255)。
 * @note 值越大，拖尾消失得越快。值越小，拖尾越长。
 */
#define FADE_RATE   64

/**
 * @brief 外部变量，用于标记流星雨动画是否已初始化。
 * @details 在 .cpp 文件中定义，确保 `init_meteor_shower()` 只被调用一次。
 */
extern bool meteor_animation_initialized;

/**
 * @brief 外部变量，流星对象池数组。
 * @details 在 .cpp 文件中定义，用于存储和管理所有流星的状态。
 */
extern Meteor meteor_pool[];


/******************************************************************************
 *                          动画函数声明 (Function Prototypes)
 ******************************************************************************/

// ======== 火焰动画 (Flame Animation) ========

/**
 * @brief (主函数) 生成一个低内存占用的逼真火焰动画。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param cooling 冷却值 (建议 20-80)，值越大，火焰熄灭越快。
 * @param sparking 火花值 (建议 50-200)，值越大，火花越多。
 * @param reversed 方向。false: 火焰向上, true: 火焰向下。
 */
void flameEffect_lowRam(SYC_WS2812& ws, int cooling, int sparking, bool reversed);

/**
 * @brief (辅助函数) 将热度值 (0-255) 转换为对应的火焰颜色。
 * @param temperature 热度值，0为最冷，255为最热。
 * @return 32位的GRB格式颜色值。
 */
uint32_t heatToColor_lowRam(byte temperature);


// ======== 彩虹动画 (Rainbow Animation) ========

/**
 * @brief 创建一个流动的彩虹效果。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param speed 控制动画的速度，值越大速度越快。
 * @param density 控制彩虹的密度，值越大颜色带越多。
 */
void anim_rainbow_flow(SYC_WS2812& ws, uint8_t speed, uint8_t density);


// ======== 帧动画 (Frame-based Animations) ========

/**
 * @brief 显示一个两帧的跳动心脏动画。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param interval 每一帧之间的间隔时间（毫秒），控制心跳速度。
 */
void anim_beating_heart(SYC_WS2812& ws, uint16_t interval);

/**
 * @brief 显示一个两帧的LOGO动画。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param interval 每一帧之间的间隔时间（毫秒），控制动画速度。
 */
void anim_logo(SYC_WS2812& ws, uint16_t interval);


// ======== 流星雨动画 (Meteor Shower Animation) ========

/**
 * @brief 渲染一个带拖尾效果的流星雨动画。
 * @param ws SYC_WS2812驱动对象的引用。
 * @param new_meteor_chance 每一帧产生新流星的概率 (0-255)，建议值为 10-40。
 */
void anim_meteor_shower(SYC_WS2812& ws, uint8_t new_meteor_chance);

/**
 * @brief (辅助函数) 初始化流星雨动画，重置所有流星状态。
 */
void init_meteor_shower(void);

#endif