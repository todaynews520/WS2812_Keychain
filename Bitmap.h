/***************************************************************************************************
 * @file      Bitmap.h
 * @author    B站 @沈已成
 * @brief     适用于 8x8 WS2812 LED矩阵的动画效果
 * @date      2025-09-25
 * @details   此文件中的位图数据按照功能进行分类，并严格按照顺序排列，
 *            以便于管理和调用。所有数据均存储在 PROGMEM 中以节省 RAM。
 *
 * @copyright Copyright (c) 2025
 *
 ***************************************************************************************************/

#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "Device.h" 
#include <stdint.h>
#include <avr/pgmspace.h>

/*
 ***************************************************************************************************
 *
 *                                  1. ❤❤❤ 动画位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 动画功能主图标 - 帧 1
extern const uint32_t Animation_logo1_num[] PROGMEM;
extern const uint8_t Animation_logo1[] PROGMEM;

// 动画功能主图标 - 帧 2
extern const uint32_t Animation_logo2_num[] PROGMEM;
extern const uint8_t Animation_logo2[] PROGMEM;

// 跳动的心 - 帧 1
extern const uint32_t heart1_num[] PROGMEM;
extern const uint8_t heart1[] PROGMEM;

// 跳动的心 - 帧 2
extern const uint32_t heart2_num[] PROGMEM;
extern const uint8_t heart2[] PROGMEM;


/*
 ***************************************************************************************************
 *
 *                                  2. ❤❤❤ 图片位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 图片功能主图标
extern const uint32_t pic_icon_num[] PROGMEM;
extern const uint8_t pic_icon_color[] PROGMEM;

// 小猫
extern const uint32_t Cat[] PROGMEM;
extern const uint8_t Cat_color[] PROGMEM;

// 桃子图片
extern const uint32_t Peach[] PROGMEM;
extern const uint8_t Peach_color[] PROGMEM;

// 心
extern const uint32_t Heart[] PROGMEM;
extern const uint8_t Heart_color[] PROGMEM;

// 鸭子图片
extern const uint32_t Dark[] PROGMEM;
extern const uint8_t Dark_color[] PROGMEM;

// 击剑图片
extern const uint32_t Sword[] PROGMEM;
extern const uint8_t Sword_color[] PROGMEM;

// 小狗图片
extern const uint32_t Dog[] PROGMEM;
extern const uint8_t Dog_color[] PROGMEM;


/*
 ***************************************************************************************************
 *
 *                                  3. ❤❤❤ 游戏位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 游戏功能主图标 (贪吃蛇)
extern const uint32_t snake_icon_num[] PROGMEM;
extern const uint8_t snake_icon_color[] PROGMEM;

// 康威生命游戏图标 - 状态 1
extern const uint32_t GOL1_num[] PROGMEM;
extern const uint8_t GOL1[] PROGMEM;

// 康威生命游戏图标 - 状态 2
extern const uint32_t GOL2_num[] PROGMEM;
extern const uint8_t GOL2[] PROGMEM;


/*
 ***************************************************************************************************
 *
 *                                  4. ❤❤❤ 字母位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 字母功能主图标
extern const uint32_t letter_icon_num[] PROGMEM;

// 字母 A
extern const uint32_t letter_a_num[] PROGMEM;

// 字母 B
extern const uint32_t letter_b_num[] PROGMEM;

// 字母 C
extern const uint32_t letter_c_num[] PROGMEM;

// 字母 D
extern const uint32_t letter_d_num[] PROGMEM;

// 字母 E
extern const uint32_t letter_e_num[] PROGMEM;

// 字母 F
extern const uint32_t letter_f_num[] PROGMEM;

// 字母 G
extern const uint32_t letter_g_num[] PROGMEM;

// 字母 H
extern const uint32_t letter_h_num[] PROGMEM;

// 字母 I
extern const uint32_t letter_i_num[] PROGMEM;

// 字母 J
extern const uint32_t letter_j_num[] PROGMEM;

// 字母 K
extern const uint32_t letter_k_num[] PROGMEM;

// 字母 L
extern const uint32_t letter_l_num[] PROGMEM;

// 字母 M
extern const uint32_t letter_m_num[] PROGMEM;

// 字母 N
extern const uint32_t letter_n_num[] PROGMEM;

// 字母 O
extern const uint32_t letter_o_num[] PROGMEM;

// 字母 P
extern const uint32_t letter_p_num[] PROGMEM;

// 字母 Q
extern const uint32_t letter_q_num[] PROGMEM;

// 字母 R
extern const uint32_t letter_r_num[] PROGMEM;

// 字母 S
extern const uint32_t letter_s_num[] PROGMEM;

// 字母 T
extern const uint32_t letter_t_num[] PROGMEM;

// 字母 U
extern const uint32_t letter_u_num[] PROGMEM;

// 字母 V
extern const uint32_t letter_v_num[] PROGMEM;

// 字母 W
extern const uint32_t letter_w_num[] PROGMEM;

// 字母 X
extern const uint32_t letter_x_num[] PROGMEM;

// 字母 Y
extern const uint32_t letter_y_num[] PROGMEM;

// 字母 Z
extern const uint32_t letter_z_num[] PROGMEM;

/*
 ***************************************************************************************************
 *
 *                                  5. ❤❤❤ 数字位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 数字功能主图标
extern const uint32_t number_icon_num[] PROGMEM;

// 数字 0
extern const uint32_t number_0_num[] PROGMEM;

// 数字 1
extern const uint32_t number_1_num[] PROGMEM;

// 数字 2
extern const uint32_t number_2_num[] PROGMEM;

// 数字 3
extern const uint32_t number_3_num[] PROGMEM;

// 数字 4
extern const uint32_t number_4_num[] PROGMEM;

// 数字 5
extern const uint32_t number_5_num[] PROGMEM;

// 数字 6
extern const uint32_t number_6_num[] PROGMEM;

// 数字 7
extern const uint32_t number_7_num[] PROGMEM;

// 数字 8
extern const uint32_t number_8_num[] PROGMEM;

// 数字 9
extern const uint32_t number_9_num[] PROGMEM;

/*
 ***************************************************************************************************
 *
 *                                  6. ❤❤❤ 工具位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 工具功能主图标
extern const uint32_t tool_icon_num[] PROGMEM;

// 亮度等级 1
extern const uint32_t LEVEL_BRIGHTNESS_num_1[] PROGMEM;
extern const uint8_t LEVEL_BRIGHTNESS_1[] PROGMEM;

// 亮度等级 2
extern const uint32_t LEVEL_BRIGHTNESS_num_2[] PROGMEM;
extern const uint8_t LEVEL_BRIGHTNESS_2[] PROGMEM;

// 亮度等级 3
extern const uint32_t LEVEL_BRIGHTNESS_num_3[] PROGMEM;
extern const uint8_t LEVEL_BRIGHTNESS_3[] PROGMEM;

// 亮度等级 4
extern const uint32_t LEVEL_BRIGHTNESS_num_4[] PROGMEM;
extern const uint8_t LEVEL_BRIGHTNESS_4[] PROGMEM;

// 亮度等级 5
extern const uint32_t LEVEL_BRIGHTNESS_num_5[] PROGMEM;
extern const uint8_t LEVEL_BRIGHTNESS_5[] PROGMEM;

/*
 ***************************************************************************************************
 *
 *                                  7. ❤❤❤ 电池状态位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 电池满电
extern const uint32_t LEVEL_FULL_num[] PROGMEM;
extern const uint8_t LEVEL_FULL_color[] PROGMEM;

// 电池高电量
extern const uint32_t LEVEL_HIGH_num[] PROGMEM;
extern const uint8_t LEVEL_HIGH_color[] PROGMEM;

// 电池中电量
extern const uint32_t LEVEL_MEDIUM_num[] PROGMEM;
extern const uint8_t LEVEL_MEDIUM_color[] PROGMEM;

// 电池低电量
extern const uint32_t LEVEL_LOW_num[] PROGMEM;
extern const uint8_t LEVEL_LOW_color[] PROGMEM;

// 电池即将耗尽 - 状态 1
extern const uint32_t LEVEL_EMPTY_num_1[] PROGMEM;
extern const uint8_t LEVEL_EMPTY_color_1[] PROGMEM;

// 电池即将耗尽 - 状态 2
extern const uint32_t LEVEL_EMPTY_num_2[] PROGMEM;
extern const uint8_t LEVEL_EMPTY_color_2[] PROGMEM;

#endif