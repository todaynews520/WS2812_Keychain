/**
 * @file Game.h
 * @author B站 沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 此头文件定义了所有内置游戏（贪吃蛇、弹珠、生命游戏）的
 * 函数接口、以及游戏板的尺寸常量。
 */

#ifndef _GAME_H_
#define _GAME_H_

#include "Device.h"

/******************************************************************************
 *                             游戏通用配置
 ******************************************************************************/

/**
 * @brief 定义游戏区域的宽度。
 */
#define BOARD_WIDTH 8
/**
 * @brief 定义游戏区域的高度。
 */
#define BOARD_HEIGHT 8


/******************************************************************************
 *                        康威生命游戏 (Game of Life)
 ******************************************************************************/
/**
 * @brief 根据一维索引获取细胞状态。
 * @param index 细胞索引 (0-63)。
 * @return 1 (存活) 或 0 (死亡)。
 */
int getCellState(int index);

/**
 * @brief 根据二维坐标获取细胞状态。
 * @param x 横坐标 (0-7)。
 * @param y 纵坐标 (0-7)。
 * @return 1 (存活) 或 0 (死亡)。
 */
int getCellStateXY(int x, int y);

/**
 * @brief 计算一个细胞周围的活邻居数量。
 * @param x 细胞的横坐标。
 * @param y 细胞的纵坐标。
 * @return 周围存活细胞的数量。
 */
int countNeighbors(int x, int y);

/**
 * @brief 计算并更新到下一代的世界状态。
 */
void computeNextGeneration(void);

/**
 * @brief 初始化生命游戏，随机生成初始世界。
 */
void initGameOfLife(void);

/**
 * @brief 生命游戏的主更新与渲染函数。
 */
void updateAndRenderGameOfLife(SYC_WS2812& ws);

/**
 * @brief 绘制生命游戏的静态切换图标。
 * @param interval 图标帧切换的间隔时间(ms)。
 */
void draw_gol_icon(SYC_WS2812& ws, uint16_t interval);


/******************************************************************************
 *                             贪吃蛇游戏 (Snake)
 ******************************************************************************/

/**
 * @brief 初始化或重置贪吃蛇游戏。
 */
void snake_init(void);

/**
 * @brief 处理贪吃蛇游戏中的按键输入。
 * @param event 传入的按键事件。
 */
void snake_handle_input(KeyEvent event);

/**
 * @brief 贪吃蛇游戏的逻辑与渲染函数。
 */
void snake_update_and_render(SYC_WS2812& ws);

/**
 * @brief 绘制贪吃蛇的动态图标。
 */
void draw_snake_icon(SYC_WS2812& ws);


/******************************************************************************
 *                             弹珠游戏 (Pinball)
 ******************************************************************************/

/**
 * @brief 初始化或重置弹珠游戏的状态。
 */
void pinball_init(void);

/**
 * @brief 处理弹珠游戏中的按键输入。
 * @param event 传入的按键事件。
 */
void pinball_handle_input(KeyEvent event);

/**
 * @brief 更新弹珠游戏的逻辑并渲染到LED矩阵上。
 * @param ws SYC_WS2812驱动对象的引用。
 */
void pinball_update_and_render(SYC_WS2812& ws);

/**
 * @brief 绘制弹珠游戏的动态LOGO。
 */
void draw_pinball_icon(SYC_WS2812& ws);

/******************************************************************************
 *                             游戏管理接口
 ******************************************************************************/

/**
 * @brief 根据指定的模式开始一个游戏。
 * @param mode 要开始的游戏模式。
 */
void game_start(GameMode mode);

/**
 * @brief 将按键输入事件分发给当前正在运行的游戏。
 * @param event 传入的按键事件。
 */
void game_handle_input(KeyEvent event);

/**
 * @brief 根据当前游戏模式，调用相应的更新和渲染函数。
 */
void game_update_and_render(SYC_WS2812& ws);

#endif