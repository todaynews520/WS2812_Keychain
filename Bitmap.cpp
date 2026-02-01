/***************************************************************************************************
 * @file      Bitmap.cpp
 * @author    B站 @沈已成
 * @brief     适用于 8x8 WS2812 LED矩阵的动画效果
 * @date      2025-09-25
 * @details   此文件中的位图数据按照功能进行分类，并严格按照顺序排列，
 *            以便于管理和调用。所有数据均存储在 PROGMEM 中以节省 RAM。
 *
 * @copyright Copyright (c) 2025
 *
 ***************************************************************************************************/

#include <WS2812_SYC_Air001.h>
#include "Bitmap.h"

/***************************************************************************************************
 *                                   位图数据存储区
 *   说明: 
 *   - 本区域存储了项目中使用的所有图形和动画的位图数据。
 *   - 数据按照以下类别进行组织，请保持此结构以便于维护：
 *     1. 动画位图
 *     2. 图片位图
 *     3. 游戏位图
 *     4. 字母位图
 *     5. 数字位图
 *     6. 工具位图
 *     7. 电池状态位图
 ***************************************************************************************************/

/*
 ***************************************************************************************************
 *
 *                                   1. ❤❤❤ 动画位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 动画功能主图标 - 帧 1
const uint32_t Animation_logo1_num[] PROGMEM = {0x0066FFFF, 0x7E3C1800};
const uint8_t Animation_logo1[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 动画功能主图标 - 帧 2
const uint32_t Animation_logo2_num[] PROGMEM = {0x0000247E, 0x3C180000};
const uint8_t Animation_logo2[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 跳动的心 - 帧 1
const uint32_t heart1_num[] PROGMEM = {0x00669981, 0x81422418};
const uint8_t heart1[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED,RED, RED, RED, RED, RED, RED};

// 跳动的心 - 帧 2
const uint32_t heart2_num[] PROGMEM = {0x0000245A, 0x42241800};
const uint8_t heart2[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

/*
 ***************************************************************************************************
 *
 *                                  2. ❤❤❤ 图片位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 图片功能主图标
const uint32_t pic_icon_num[] PROGMEM = {0x3C42A5A5, 0x8199423C};
const uint8_t pic_icon_color[] PROGMEM = {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, RED, RED, YELLOW, YELLOW, YELLOW, YELLOW, RED, RED, YELLOW, YELLOW, RED, RED, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW};

// 小猫
const uint32_t Cat[] PROGMEM = {0x81C3FFFF, 0xFFFFFF7E};
const uint8_t Cat_color[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, WHITE, WHITE, RED, RED, RED, RED, WHITE, RED, RED, RED, WHITE, RED, RED, RED, WHITE, WHITE, RED, RED, WHITE, WHITE, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 桃子
const uint32_t Peach[] PROGMEM = {0x00183C7E, 0xFFFF7E24};
const uint8_t Peach_color[] PROGMEM = {GREEN, GREEN, GREEN, PINK, PINK, PINK, PINK, GREEN, GREEN, PINK, PINK, PINK, PINK, PINK, PINK, GREEN, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK, PINK};

// 心
const uint32_t Heart[] PROGMEM = {0x0066FFFF, 0x7E3C1800};
const uint8_t Heart_color[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 鸭子
const uint32_t Dark[] PROGMEM = {0x1C3E3E3F, 0x9CFEFE7E};
const uint8_t Dark_color[] PROGMEM = {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, ORANGE, ORANGE, ORANGE, YELLOW, YELLOW, YELLOW, ORANGE, YELLOW, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW};

// 击剑
const uint32_t Sword[] PROGMEM = {0xC3E77E3C, 0xBD7E6699};
const uint8_t Sword_color[] PROGMEM = {WHITE, BLUE, RED, WHITE, WHITE, BLUE, RED, WHITE, BLUE, BLUE, BLUE, RED, RED, RED, BLUE, BLUE, RED, RED, RED, RED, RED, RED, RED, BLUE, RED, RED, RED, BLUE, BLUE, BLUE, RED, RED, RED, BLUE, BLUE, BLUE, RED, RED, BLUE, BLUE};

// 小狗
const uint32_t Dog[] PROGMEM = {0x00507072, 0x7E7E7E52};
const uint8_t Dog_color[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, RED, WHITE, ORANGE, ORANGE, WHITE, WHITE, BLUE, WHITE, ORANGE, BLUE, ORANGE, BLUE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE};

/*
 ***************************************************************************************************
 *
 *                                  3. ❤❤❤ 游戏位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 游戏功能主图标 (贪吃蛇)
const uint32_t snake_icon_num[] PROGMEM = {0x007C4464, 0x24042600};
const uint8_t snake_icon_color[] PROGMEM = {RED, RED, GREEN, RED, RED, WHITE, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 康威生命游戏图标 - 状态 1
const uint32_t GOL1_num[] PROGMEM = {0x24009924, 0x24990024};
const uint8_t GOL1[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

// 康威生命游戏图标 - 状态 2
const uint32_t GOL2_num[] PROGMEM = {0x180018A5, 0xA5180018};
const uint8_t GOL2[] PROGMEM = {RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED, RED};

/*
 ***************************************************************************************************
 *
 *                                  4. ❤❤❤ 字母位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 字母功能主图标
const uint32_t letter_icon_num[] PROGMEM = {0x46A5A5E6, 0xA5A5A600};

// 字母 A
const uint32_t letter_a_num[] PROGMEM = {0x00182424, 0x3C242424};

// 字母 B
const uint32_t letter_b_num[] PROGMEM = {0x00382424, 0x38242438};

// 字母 C
const uint32_t letter_c_num[] PROGMEM = {0x00182420, 0x20202418};

// 字母 D
const uint32_t letter_d_num[] PROGMEM = {0x00382424, 0x24242438};

// 字母 E
const uint32_t letter_e_num[] PROGMEM = {0x003C2020, 0x3820203C};

// 字母 F
const uint32_t letter_f_num[] PROGMEM = {0x003C2020, 0x3C202020};

// 字母 G
const uint32_t letter_g_num[] PROGMEM = {0x00182420, 0x2C242418};

// 字母 H
const uint32_t letter_h_num[] PROGMEM = {0x00242424, 0x3C242424};

// 字母 I
const uint32_t letter_i_num[] PROGMEM = {0x003C1818, 0x1818183C};

// 字母 J
const uint32_t letter_j_num[] PROGMEM = {0x001C0404, 0x04042418};

// 字母 K
const uint32_t letter_k_num[] PROGMEM = {0x00242428, 0x30282424};

// 字母 L
const uint32_t letter_l_num[] PROGMEM = {0x00202020, 0x2020203C};

// 字母 M
const uint32_t letter_m_num[] PROGMEM = {0x00004266, 0x5A424242};

// 字母 N
const uint32_t letter_n_num[] PROGMEM = {0x00002424, 0x342C2424};

// 字母 O
const uint32_t letter_o_num[] PROGMEM = {0x00001824, 0x24242418};

// 字母 P
const uint32_t letter_p_num[] PROGMEM = {0x00003824, 0x24382020};

// 字母 Q
const uint32_t letter_q_num[] PROGMEM = {0x00001824, 0x24242C1C};

// 字母 R
const uint32_t letter_r_num[] PROGMEM = {0x00003824, 0x24382424};

// 字母 S
const uint32_t letter_s_num[] PROGMEM = {0x00182420, 0x18042418};

// 字母 T
const uint32_t letter_t_num[] PROGMEM = {0x00003C18, 0x18181818};

// 字母 U
const uint32_t letter_u_num[] PROGMEM = {0x00002424, 0x24242418};

// 字母 V
const uint32_t letter_v_num[] PROGMEM = {0x00004242, 0x42422418};

// 字母 W
const uint32_t letter_w_num[] PROGMEM = {0x00004242, 0x425A6642};

// 字母 X
const uint32_t letter_x_num[] PROGMEM = {0x00444428, 0x10284444};

// 字母 Y
const uint32_t letter_y_num[] PROGMEM = {0x00444428, 0x10101010};

// 字母 Z
const uint32_t letter_z_num[] PROGMEM = {0x00003C04, 0x0810203C};

/*
 ***************************************************************************************************
 *
 *                                  5. ❤❤❤ 数字位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 数字功能主图标
const uint32_t number_icon_num[] PROGMEM = {0x00E2A6A2, 0xA2A2E700};

// 数字 0
const uint32_t number_0_num[] PROGMEM = {0x00182424, 0x24242418};

// 数字 1
const uint32_t number_1_num[] PROGMEM = {0x00183818, 0x1818183C};

// 数字 2
const uint32_t number_2_num[] PROGMEM = {0x00182404, 0x0810203C};

// 数字 3
const uint32_t number_3_num[] PROGMEM = {0x00182404, 0x08042418};

// 数字 4
const uint32_t number_4_num[] PROGMEM = {0x00182828, 0x283C0808};

// 数字 5
const uint32_t number_5_num[] PROGMEM = {0x003C2038, 0x04042418};

// 数字 6
const uint32_t number_6_num[] PROGMEM = {0x00182420, 0x38242418};

// 数字 7
const uint32_t number_7_num[] PROGMEM = {0x003C2404, 0x08081010};

// 数字 8
const uint32_t number_8_num[] PROGMEM = {0x00182424, 0x18242418};

// 数字 9
const uint32_t number_9_num[] PROGMEM = {0x00182424, 0x1C040810};

/*
 ***************************************************************************************************
 *
 *                                  6. ❤❤❤ 工具位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 工具功能主图标
const uint32_t tool_icon_num[] PROGMEM = {0x1C382123, 0x337EE0C0};

// 亮度等级 1
const uint32_t LEVEL_BRIGHTNESS_num_1[] PROGMEM = {0x18280808, 0x0808083E};
const uint8_t LEVEL_BRIGHTNESS_1[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 亮度等级 2
const uint32_t LEVEL_BRIGHTNESS_num_2[] PROGMEM = {0x1C222204, 0x0810203E};
const uint8_t LEVEL_BRIGHTNESS_2[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 亮度等级 3
const uint32_t LEVEL_BRIGHTNESS_num_3[] PROGMEM = {0x1C22020C, 0x0222221C};
const uint8_t LEVEL_BRIGHTNESS_3[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 亮度等级 4
const uint32_t LEVEL_BRIGHTNESS_num_4[] PROGMEM = {0x10244444, 0x7E040404};
const uint8_t LEVEL_BRIGHTNESS_4[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 亮度等级 5
const uint32_t LEVEL_BRIGHTNESS_num_5[] PROGMEM = {0x3E20203C, 0x0202023C};
const uint8_t LEVEL_BRIGHTNESS_5[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

/*
 ***************************************************************************************************
 *
 *                                  7. ❤❤❤ 电池状态位图 ❤❤❤
 *
 ***************************************************************************************************
 */

// 电池满电
const uint32_t LEVEL_FULL_num[] PROGMEM = {0x3C7E7E7E, 0x7E7E7E7E};
const uint8_t LEVEL_FULL_color[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 电池高电量
const uint32_t LEVEL_HIGH_num[] PROGMEM = {0x3C7E427E, 0x7E7E7E7E};
const uint8_t LEVEL_HIGH_color[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, GREEN, GREEN, GREEN, GREEN, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 电池中电量
const uint32_t LEVEL_MEDIUM_num[] PROGMEM = {0x3C7E4242, 0x7E7E7E7E};
const uint8_t LEVEL_MEDIUM_color[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, WHITE, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, WHITE, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 电池低电量
const uint32_t LEVEL_LOW_num[] PROGMEM = {0x3C7E4242, 0x427E7E7E};
const uint8_t LEVEL_LOW_color[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, WHITE, WHITE, YELLOW, YELLOW, YELLOW, YELLOW, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 电池即将耗尽 - 状态 1
const uint32_t LEVEL_EMPTY_num_1[] PROGMEM = {0x3C7E4242, 0x42427E7E};
const uint8_t LEVEL_EMPTY_color_1[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, RED, RED, RED, RED, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};

// 电池即将耗尽 - 状态 2
const uint32_t LEVEL_EMPTY_num_2[] PROGMEM = {0x3C7E4242, 0x4242427E};
const uint8_t LEVEL_EMPTY_color_2[] PROGMEM = {WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE};