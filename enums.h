#ifndef _ENUMS_H_
#define _ENUMS_H_

#include <pgmspace.h> // 包含这个头文件来使用 PROGMEM

// 主模式
enum class MainMode {
    ANIMATION,
    PIC,
    GAME,
    LETTER,
    NUMBER,
    TOOL,
    // 特殊全屏模式，优先级最高
    SYSTEM_OVERLAY 
};

// 卡通人物模式
enum class PicMode {
    CAT,
    PEACH,
    HEART,
    DARK,
    SWORD,
    DOG
};

enum class LetterMode {
    Letter_A,
    Letter_B,
    Letter_C,
    Letter_D,
    Letter_E,
    Letter_F,
    Letter_G,
    Letter_H,
    Letter_I,
    Letter_J,
    Letter_K,
    Letter_L,
    Letter_M,
    Letter_N,
    Letter_O,
    Letter_P,
    Letter_Q,
    Letter_R,
    Letter_S,
    Letter_T,
    Letter_U,
    Letter_V,
    Letter_W,
    Letter_X,
    Letter_Y,
    Letter_Z
};

enum class NumberMode {
    Number_0,
    Number_1,
    Number_2,
    Number_3,
    Number_4,
    Number_5,
    Number_6,
    Number_7,
    Number_8,
    Number_9
};

// 动画子模式
enum class AnimMode {
    FLAME,
    RAINBOW,
    RAINBOW_HEART,
    METEOR
};

// 游戏子模式
enum class GameMode {
    PINBALL,
    SNAKE,
    GAME_OF_LIFE
};

// 工具子模式
enum class ToolMode {
    SETTINGS
};

// 系统覆盖模式 (用于充电、电量显示等)
enum class SystemOverlayMode {
    NONE,
    BATTERY_DISPLAY,
    CHARGING_ANIMATION,
    CHARGING,
    CHARGE_FULL,
    LOW_POWER_WARNING
};

// 按键事件
enum class KeyEvent {
    NO_EVENT, 
    LEFT_CLICK,
    LEFT_LONG_PRESS,
    RIGHT_CLICK, 
    RIGHT_LONG_PRESS, 
    BOTH_PRESS
};
/**
 * @brief 电池电量等级 (4档)
 */
enum BatteryLevel {
    LEVEL_EMPTY,        // 0-10% (需要提示低电量)
    LEVEL_LOW,          // 10-40%
    LEVEL_MEDIUM,       // 40-75%
    LEVEL_HIGH,
    LEVEL_FULL          // 75-100%
};
/**
 * @brief 充电状态
 */
enum ChargingState {
    STATE_DISCHARGING, // 未连接充电器，正在放电
    STATE_CHARGING,    // 正在充电
    STATE_CHARGE_FULL  // 连接着充电器，但已充满
};

struct AppState {
    MainMode main_mode;
    AnimMode anim_mode;
    PicMode pic_mode;
    GameMode game_mode;
    LetterMode letter_mode;
    NumberMode number_mode;
    ToolMode tool_mode;
    SystemOverlayMode overlay_mode;

    bool in_sub_menu;
    bool is_game_running; 

    uint8_t brightness_level;
};

// 流星结构体
struct Meteor {
    int16_t x;           // 定点数坐标 (Q8.8: 8位整数, 8位小数)
    int16_t y;
    int16_t speed;
    uint8_t hue;
    bool is_active;
};

// 使用 extern 关键字进行声明
extern AppState appState;

#endif