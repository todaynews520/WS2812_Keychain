/**
 * @file Game.cpp
 * @author B站 沈已成
 * @brief 适用于 8x8 WS2812 LED矩阵的动画效果
 * @date 2025-09-25
 *
 * @copyright Copyright (c) 2025
 *
 * 本文件包含了多个内置小游戏的完整逻辑，包括：
 * - 弹珠游戏 (Pinball)
 * - 康威生命游戏 (Conway's Game of Life)
 * - 贪吃蛇 (Snake)
 * 并为每个游戏提供了初始化、输入处理、逻辑更新和屏幕渲染的函数。
 * 文件末尾还提供了统一的游戏管理接口。
 */

#include "Game.h"

/******************************************************************************
 *                            弹珠游戏 (Pinball)
 ******************************************************************************/

// 弹珠游戏的状态枚举
enum PinballState {
    PINBALL_RUNNING,    // 游戏运行中
    PINBALL_GAME_OVER   // 游戏结束
};
// 静态变量，保存当前弹珠游戏的状态
static PinballState pinball_state;

// --- 游戏对象 ---
static int ball_x, ball_y;         // 小球的坐标 (x, y)
static int vel_x, vel_y;           // 小球的速度向量 (x方向速度, y方向速度)
static int paddle_pos;             // 挡板的左端x坐标
const int PADDLE_LEN = 3;          // 挡板的长度（3个像素）

// --- 用于动态LOGO的游戏对象  ---
static int logo_ball_x, logo_ball_y;        // LOGO小球坐标
static int logo_vel_x, logo_vel_y;          // LOGO小球速度
static int logo_paddle_pos;                 // LOGO挡板位置
static unsigned long logo_last_update_time; // LOGO上次更新时间
static bool logo_is_initialized = false;    // LOGO动画状态

// --- 游戏通用参数 ---
static unsigned long game_time;     // 记录上一帧的时间，用于控制更新速率
static int game_speed;              // 游戏速度 (帧更新间隔，单位ms，值越小越快)
static uint8_t rainbow_hue = 0;     // 生成彩虹色，随时间变化

// --- Game Over 状态计时 ---
static unsigned long game_over_time; // 记录进入Game Over状态的时刻

/**
 * @brief (弹珠游戏辅助函数) 将二维坐标(x, y)转换为一维的LED索引。
 * @param x 横坐标 (0-7)。
 * @param y 纵坐标 (0-7)。
 * @return 对应的LED索引 (0-63)。如果坐标无效，则返回-1。
 */
static int pos2index(int x, int y) {
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return -1; // 边界检查，返回无效索引
    }
    // 按行排列
    return y * BOARD_WIDTH + x;
}

/**
 * @brief 初始化或重置弹珠游戏的状态。
 */
void pinball_init() {
    ball_x = 2;                      // 小球初始横坐标
    ball_y = 2;                      // 小球初始纵坐标
    vel_x = 1;                       // x方向初始速度
    vel_y = 1;                       // y方向初始速度
    paddle_pos = 2;                  // 挡板初始位置
    game_speed = 300;                // 初始游戏速度
    pinball_state = PINBALL_RUNNING; // 设置游戏状态为运行中
    game_time = millis();            // 初始化游戏计时器
}

/**
 * @brief 处理弹珠游戏中的按键输入。
 * @param event 传入的按键事件。
 */
void pinball_handle_input(KeyEvent event) {
    // 如果当前是 "Game Over" 状态
    if (pinball_state == PINBALL_GAME_OVER) {
        // 任意单击事件都会重新开始游戏
        if (event == KeyEvent::LEFT_CLICK || event == KeyEvent::RIGHT_CLICK) {
            pinball_init(); // 重新初始化
        }
        return;
    }

    // 控制挡板移动
    if (event == KeyEvent::LEFT_CLICK) {
        paddle_pos--; // 左键，挡板左移
    }
    else if (event == KeyEvent::RIGHT_CLICK) {
        paddle_pos++; // 右键，挡板右移
    }
    // 确保挡板不会移出屏幕边界
    paddle_pos = constrain(paddle_pos, 0, BOARD_WIDTH - PADDLE_LEN);
}

/**
 * @brief 渲染弹珠游戏。
 */
void pinball_update_and_render(SYC_WS2812& ws) {
    if (pinball_state == PINBALL_RUNNING) {
        if (millis() - game_time > game_speed) {
            game_time = millis(); // 更新计时器

            // 1. 小球位置更新
            ball_x += vel_x;
            ball_y += vel_y;

            // 2. 碰撞检测
            // 与左右墙壁碰撞，x速度反向
            if (ball_x >= BOARD_WIDTH - 1 || ball_x <= 0) {
                vel_x = -vel_x;
            }
            // 与上墙壁碰撞，y速度反向
            if (ball_y <= 0) {
                vel_y = -vel_y;
            }

            // 挡板碰撞检测 (在倒数第二行进行)
            if (ball_y >= BOARD_HEIGHT - 2) {
                // 判断小球的x坐标是否落在挡板的长度范围内
                if (ball_x >= paddle_pos && ball_x < paddle_pos + PADDLE_LEN) {
                    vel_y = -vel_y; // 接住了，y速度反向
                    if (game_speed > 60) { // 如果速度还没到最快
                        game_speed -= 20; // 游戏加速
                    }
                } else {
                    // 没接住
                    pinball_state = PINBALL_GAME_OVER; // 切换到游戏结束状态
                    game_over_time = millis();         // 记录游戏结束的时刻
                }
            }
        }
    }

    // --- 渲染 ---
    rainbow_hue++; // 色相随时间递增，用于彩虹效果

    if (pinball_state == PINBALL_RUNNING) {
        // 绘制挡板 (白色)
        for (int i = 0; i < PADDLE_LEN; i++) {
            ws.setWs2812Color(pos2index(paddle_pos + i, BOARD_HEIGHT - 1), WHITE_Color);
        }
        // 绘制小球 (彩虹色)
        ws.setWs2812Color(pos2index(ball_x, ball_y), ws.Wheel(rainbow_hue));
    }
    else if (pinball_state == PINBALL_GAME_OVER) {
        // 绘制 Game Over 闪烁效果 (全屏红色闪烁)
        if ((millis() / 300) % 2 == 0) {
            for (int i = 0; i < ws2812_number; i++) {
                ws.setWs2812Color(i, RED_Color);
            }
        }
    }
}

/**
 * @brief 绘制弹珠游戏的动态LOGO。
 */
void draw_pinball_icon(SYC_WS2812& ws) {
    const int UPDATE_INTERVAL = 150;       // LOGO动画的更新间隔

    // --- 初始化 ---
    if (!logo_is_initialized) {
        logo_ball_x = 3; logo_ball_y = 2;  // LOGO小球初始位置
        logo_vel_x = 1; logo_vel_y = 1;    // LOGO小球初始速度
        logo_paddle_pos = 2;               // LOGO挡板初始位置
        logo_last_update_time = millis();  // 初始化计时器
        logo_is_initialized = true;        // 设置初始化标志
    }

    // --- 逻辑更新 ---
    if (millis() - logo_last_update_time > UPDATE_INTERVAL) {
        logo_last_update_time = millis();

        // a. 小球位置更新
        logo_ball_x += logo_vel_x;
        logo_ball_y += logo_vel_y;

        // b. 小球碰撞检测
        // 左右墙壁反弹
        if (logo_ball_x >= BOARD_WIDTH - 1 || logo_ball_x <= 0) {
            logo_vel_x = -logo_vel_x;
        }
        // 上墙壁反弹
        if (logo_ball_y <= 0) {
            logo_vel_y = -logo_vel_y;
        }
        // 挡板反弹
        if (logo_ball_y >= BOARD_HEIGHT - 1) {
            logo_vel_y = -logo_vel_y;
        }

        int target_paddle_pos = logo_ball_x - 1;
        target_paddle_pos = constrain(target_paddle_pos, 0, BOARD_WIDTH - PADDLE_LEN); // 限制在边界内

        if (logo_paddle_pos < target_paddle_pos) {
            logo_paddle_pos++;
        } else if (logo_paddle_pos > target_paddle_pos) {
            logo_paddle_pos--;
        }
    }

    // --- 渲染 ---
    // a. 绘制小球 (彩虹色)
    ws.setWs2812Color(pos2index(logo_ball_x, logo_ball_y), ws.Wheel(millis() / 20));
    // b. 绘制挡板 (白色)
    for (int i = 0; i < PADDLE_LEN; i++) {
        ws.setWs2812Color(pos2index(logo_paddle_pos + i, BOARD_HEIGHT - 1), WHITE_Color);
    }
}


/******************************************************************************
 *                        康威生命游戏 (Conway's Game of Life)
 ******************************************************************************/

// ---- 游戏配置与变量 ----
const int GOL_UPDATE_INTERVAL = 200; // 每一代演化的间隔时间 (ms)

// world[0] 存储 0-31 号细胞的状态，world[1] 存储 32-63 号
uint32_t life_world[2] = {0, 0};

// ---- 游戏流程控制 ----
unsigned long gol_last_update_time = 0; // 上次演化的时间戳

/**
 * @brief 根据一维索引获取细胞状态。
 * @param index 细胞索引 (0-63)。
 * @return 1 表示存活，0 表示死亡。
 */
int getCellState(int index) {
    if (index < 0 || index >= 64) return 0; // 边界检查
    
    if (index < 32) {
        // 在第一个整数中，通过位运算获取状态
        return (life_world[0] >> index) & 1;
    } else {
        // 在第二个整数中，索引要相应调整
        return (life_world[1] >> (index - 32)) & 1;
    }
}

/**
 * @brief 根据二维坐标获取细胞状态。
 * @param x 横坐标 (0-7)。
 * @param y 纵坐标 (0-7)。
 * @return 1 表示存活，0 表示死亡。
 */
int getCellStateXY(int x, int y) {
    if (x < 0 || x >= 8 || y < 0 || y >= 8) {
        return 0; // 边界之外视为死亡细胞
    }
    // 灯珠为行式排布，计算对应的一维索引
    int index = y * 8 + x;
    return getCellState(index);
}

/**
 * @brief 计算指定细胞周围的8个邻居中有多少个是活的。
 * @param x 细胞的横坐标。
 * @param y 细胞的纵坐标。
 * @return 存活邻居的数量。
 */
int countNeighbors(int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // 跳过细胞本身
            count += getCellStateXY(x + i, y + j);
        }
    }
    return count;
}

/**
 * @brief 根据生命游戏规则计算下一代的世界状态。
 */
void computeNextGeneration() {
    uint32_t next_world[2] = {0, 0}; // 创建一个临时的数组来存储下一代的状态

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            int index = y * 8 + x; // 同样使用列式排布的索引
            
            int neighbors = countNeighbors(x, y);
            int current_state = getCellState(index);
            bool is_alive_next = false; // 默认为死亡

            // 应用康威生命游戏的三条核心规则
            // 规则1 & 2: 活细胞在邻居为2或3时继续存活
            if (current_state == 1 && (neighbors == 2 || neighbors == 3)) {
                is_alive_next = true;
            } 
            // 规则3: 死细胞在邻居恰好为3时复活
            else if (current_state == 0 && neighbors == 3) {
                is_alive_next = true;
            }

            // 如果下一代该细胞是存活的，则在 next_world 的相应位上置1
            if (is_alive_next) {
                if (index < 32) {
                    next_world[0] |= (1UL << index);
                } else {
                    next_world[1] |= (1UL << (index - 32));
                }
            }
        }
    }
    
    // 用计算出的新世界覆盖当前世界
    life_world[0] = next_world[0];
    life_world[1] = next_world[1];
}

/**
 * @brief 初始化或重置生命游戏，随机生成初始细胞图案。
 */
void initGameOfLife() {
    life_world[0] = 0; // 清空世界
    life_world[1] = 0;
    // 随机填充约20%的细胞作为初始状态
    for (int i = 0; i < 64 / 5; i++) {
        int index = random(64);
        if (index < 32) {
            life_world[0] |= (1UL << index);
        } else {
            life_world[1] |= (1UL << (index - 32));
        }
    }
}

/**
 * @brief 生命游戏的主更新与渲染函数。
 */
void updateAndRenderGameOfLife(SYC_WS2812& ws) {
    // --- 渲染当前世界 ---
    for (int i = 0; i < 64; i++) {
        if (getCellState(i)) {
            ws.setWs2812Color(i, RED_Color); // 活细胞为红色
        } else {
            ws.setWs2812Color(i, BLACK_Color); // 死细胞为黑色
        }
    }
    ws.Ws2812_show(); // 推送颜色数据到LED

    // --- 定时演化下一代 ---
    if (millis() - gol_last_update_time > GOL_UPDATE_INTERVAL) {
        gol_last_update_time = millis();
        
        // 保存当前世界状态，用于检测演化是否停滞
        uint32_t old_world[2] = {life_world[0], life_world[1]};
        
        computeNextGeneration(); // 计算下一代

        // 简化的停滞检测：如果世界全灭，或者状态不再变化，则重新开始
        if ((life_world[0] == 0 && life_world[1] == 0) ||
            (life_world[0] == old_world[0] && life_world[1] == old_world[1])) {
            initGameOfLife();
        }
    }
}

/**
 * @brief 绘制生命游戏的静态切换图标。
 * @param interval 图标帧切换的间隔时间(ms)。
 */
void draw_gol_icon(SYC_WS2812& ws, uint16_t interval) {
    static unsigned long last_frame_time = 0;
    static uint8_t current_frame = 0;

    // 定时切换帧
    if (millis() - last_frame_time >= interval) {
        last_frame_time = millis();
        current_frame = (current_frame + 1) % 2; 
    }
    
    // 根据当前帧数绘制不同的位图
    if (current_frame == 0) {
        ws.Draw(GOL1_num, GOL1);
    } else {
        ws.Draw(GOL2_num, GOL2);
    }
}


/******************************************************************************
 *                            贪吃蛇游戏 (Snake)
 ******************************************************************************/
#define SNAKE_MAX_LENGTH 64 // 蛇的最大长度

// 贪吃蛇游戏状态枚举
enum class SnakeState { IDLE, RUNNING, GAME_OVER };

// 蛇的前进方向枚举
enum class SnakeDirection { UP, DOWN, LEFT, RIGHT };

// --- 游戏变量 ---
SnakeState snake_state;         // 当前游戏状态
SnakeDirection snake_dir;       // 当前前进方向

// 使用坐标结构体来表示一个点
struct Point { int8_t x; int8_t y; };

Point snake_body[SNAKE_MAX_LENGTH]; // 数组，存储蛇身每一节的坐标
uint8_t snake_len;                  // 蛇的当前长度
Point food;                         // 食物的坐标

unsigned long snake_last_move_time; // 上次移动的时间戳
int snake_move_interval = 350;      // 移动的时间间隔 (ms)

/**
 * @brief 初始化或重置贪吃蛇游戏。
 */
void snake_init() {
    snake_state = SnakeState::RUNNING; // 设置状态为运行中
    snake_len = 3;                     // 初始长度为3
    // 初始化蛇身在屏幕中间
    snake_body[0] = {4, 4}; // 头
    snake_body[1] = {3, 4};
    snake_body[2] = {2, 4};
    snake_dir = SnakeDirection::RIGHT; // 初始方向向右
    
    // 随机生成一个食物
    food = {random(BOARD_WIDTH), random(BOARD_HEIGHT)};

    snake_last_move_time = millis(); // 重置移动计时器
}

/**
 * @brief 处理贪吃蛇游戏中的按键输入。
 * @param event 传入的按键事件。
 */
void snake_handle_input(KeyEvent event) {
    if (snake_state == SnakeState::GAME_OVER) {
        // 如果游戏结束，任意单击事件都将重新开始
        if (event == KeyEvent::LEFT_CLICK || event == KeyEvent::RIGHT_CLICK) {
            snake_init();
        }
        return;
    }

    // 左键控制蛇
    if (event == KeyEvent::LEFT_CLICK) {
        switch(snake_dir) {
            case SnakeDirection::UP:    snake_dir = SnakeDirection::LEFT; break;
            case SnakeDirection::DOWN:  snake_dir = SnakeDirection::LEFT; break;
            case SnakeDirection::LEFT:  snake_dir = SnakeDirection::DOWN; break;
            case SnakeDirection::RIGHT: snake_dir = SnakeDirection::UP; break;
        }
    } 
    // 右键控制蛇
    else if (event == KeyEvent::RIGHT_CLICK) {
        switch(snake_dir) {
            case SnakeDirection::UP:    snake_dir = SnakeDirection::RIGHT; break;
            case SnakeDirection::DOWN:  snake_dir = SnakeDirection::RIGHT; break;
            case SnakeDirection::LEFT:  snake_dir = SnakeDirection::UP; break;
            case SnakeDirection::RIGHT: snake_dir = SnakeDirection::DOWN; break;
        }
    }
}

/**
 * @brief 贪吃蛇游戏的逻辑与渲染函数。
 */
void snake_update_and_render(SYC_WS2812& ws) {
    ws.clearWs2812(); // 每帧开始时清空屏幕
    
    // -- 1. 逻辑更新 (基于时间间隔) --
    if (snake_state == SnakeState::RUNNING && millis() - snake_last_move_time > snake_move_interval) {
        snake_last_move_time = millis();

        Point next_head = snake_body[0]; // 获取当前蛇头的位置

        // 根据前进方向计算下一帧蛇头的新位置
        if (snake_dir == SnakeDirection::UP)    next_head.y--;
        if (snake_dir == SnakeDirection::DOWN)  next_head.y++;
        if (snake_dir == SnakeDirection::LEFT)  next_head.x--;
        if (snake_dir == SnakeDirection::RIGHT) next_head.x++;

        // 检查碰撞
        // a. 撞墙
        if (next_head.x < 0 || next_head.x >= BOARD_WIDTH || next_head.y < 0 || next_head.y >= BOARD_HEIGHT) {
            snake_state = SnakeState::GAME_OVER;
        }
        // b. 撞到自己
        for (int i = 0; i < snake_len; i++) {
            if (next_head.x == snake_body[i].x && next_head.y == snake_body[i].y) {
                snake_state = SnakeState::GAME_OVER;
                break;
            }
        }
        
        // 如果游戏已结束，则跳过后续的移动和吃食物逻辑
        if (snake_state == SnakeState::GAME_OVER) {

        } else {
            // c. 吃到食物
            bool ate_food = (next_head.x == food.x && next_head.y == food.y);
            if (ate_food) {
                if (snake_len < SNAKE_MAX_LENGTH) {
                    snake_len++; // 蛇身变长
                }
                // 在新位置生成食物
                food = {random(BOARD_WIDTH), random(BOARD_HEIGHT)};
            }

            // 移动蛇身 (核心)
            // 从尾部开始，将每一节身体移动到其前一节的位置
            for (int i = snake_len - 1; i > 0; i--) {
                snake_body[i] = snake_body[i-1];
            }
            snake_body[0] = next_head;
        }
    }

    // -- 2. 渲染 --
    if (snake_state == SnakeState::RUNNING) {
        // 渲染蛇身，蛇头为白色，身体为红色
        for (int i = 0; i < snake_len; i++) {
            int index = snake_body[i].y * BOARD_WIDTH + snake_body[i].x; 
            ws.setWs2812Color(index, i == 0 ? WHITE_Color : RED_Color);
        }
        // 渲染食物 (绿色)
        if( (millis()/200) % 2 == 0) {
            int food_index = food.y * BOARD_WIDTH + food.x;
                ws.setWs2812Color(food_index, GREEN_Color);
        }
    } else if (snake_state == SnakeState::GAME_OVER) {
        // 游戏结束时，全屏红色闪烁
        if ((millis() / 300) % 2 == 0) {
            for(int i=0; i<64; i++) ws.setWs2812Color(i, RED_Color);
        }
    }
}

/**
 * @brief 绘制贪吃蛇的动态图标。
 * @param ws SYC_WS2812驱动对象的引用。
 */
void draw_snake_icon(SYC_WS2812& ws)
{
    // --- Logo动画配置 ---
    const int SNAKE_LENGTH = 5;         // 图标中蛇的长度
    const int ANIMATION_INTERVAL = 150; // 移动间隔
    
    // 定义蛇的运动路径范围
    const int PATH_MIN_COORD = 1;
    const int PATH_MAX_COORD = BOARD_WIDTH - 2;

    // 定义一个固定的食物位置
    const Point FIXED_APPLE_POSITION = {6, 3};

    // --- 静态变量，保存动画状态 ---
    static bool is_initialized = false;
    static Point snake_body[SNAKE_LENGTH];
    static int direction;
    static unsigned long last_move_time;
    static bool is_apple_active;
    
    // --- 初始化逻辑 ---
    if (!is_initialized) {
        // 设置蛇的初始位置和方向
        for (int i = 0; i < SNAKE_LENGTH; i++) {
            snake_body[i] = { (float)(SNAKE_LENGTH - i + PATH_MIN_COORD - 1), PATH_MIN_COORD };
        }
        direction = 0; 
        is_apple_active = true; // 初始时苹果可见
        last_move_time = millis();
        is_initialized = true;
    }

    // --- 动画逻辑更新 ---
    if (millis() - last_move_time > ANIMATION_INTERVAL) {
        last_move_time = millis();

        Point current_head = snake_body[0];
        Point next_head = current_head;

        bool just_completed_lap = false;
        if (direction == 0 && current_head.x == PATH_MAX_COORD)      direction = 1;
        else if (direction == 1 && current_head.y == PATH_MAX_COORD) direction = 2;
        else if (direction == 2 && current_head.x == PATH_MIN_COORD) direction = 3;
        else if (direction == 3 && current_head.y == PATH_MIN_COORD) {             
            direction = 0;
            just_completed_lap = true;
        }
        
        if (direction == 0) next_head.x++;
        if (direction == 1) next_head.y++;
        if (direction == 2) next_head.x--;
        if (direction == 3) next_head.y--;

        if (is_apple_active && next_head.x == FIXED_APPLE_POSITION.x && next_head.y == FIXED_APPLE_POSITION.y) {
            is_apple_active = false;
        }

        for (int i = SNAKE_LENGTH - 1; i > 0; i--) {
            snake_body[i] = snake_body[i - 1];
        }
        snake_body[0] = next_head;

        if (just_completed_lap) {
            is_apple_active = true;
        }
    }
    
    // --- 渲染 ---
    ws.clearWs2812();

    if (is_apple_active && (millis() / 250) % 2 == 0) {
        int apple_index = FIXED_APPLE_POSITION.y * BOARD_WIDTH + FIXED_APPLE_POSITION.x;
        ws.setWs2812Color(apple_index, GREEN_Color); 
    }

    // 渲染蛇身
    for (int i = 0; i < SNAKE_LENGTH; i++) {
        int index = snake_body[i].y * BOARD_WIDTH + snake_body[i].x;
        if (index >= 0 && index < BOARD_WIDTH * BOARD_HEIGHT) {
            ws.setWs2812Color(index, i == 0 ? WHITE_Color : RED_Color);
        }
    }
}


/******************************************************************************
 *                             游戏管理接口
 ******************************************************************************/

/**
 * @brief 根据指定的模式开始一个游戏。
 * @param mode 要开始的游戏模式。
 */
void game_start(GameMode mode) {
    switch(mode) {
        case GameMode::SNAKE:
            snake_init();
            break;
        case GameMode::PINBALL:
            pinball_init();
            break;
        case GameMode::GAME_OF_LIFE:
            initGameOfLife();
            break;
    }
}

/**
 * @brief 将按键输入事件分发给当前正在运行的游戏。
 * @param event 传入的按键事件。
 */
void game_handle_input(KeyEvent event) {
    switch(appState.game_mode) {
        case GameMode::SNAKE:
            snake_handle_input(event);
            break;
        case GameMode::PINBALL:
            pinball_handle_input(event);
            break;
    }
}

/**
 * @brief 根据当前游戏模式，调用相应的更新和渲染函数。
 */
void game_update_and_render(SYC_WS2812& ws) {
    switch(appState.game_mode) {
        case GameMode::SNAKE:
            snake_update_and_render(ws);
            break;
        case GameMode::PINBALL:
            pinball_update_and_render(ws);
            break;
        case GameMode::GAME_OF_LIFE:
            updateAndRenderGameOfLife(ws);
            break;
    }
}