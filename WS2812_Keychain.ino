#include "Device.h"
#include "manage.h"

/* 项目框架(4层) 

1、底层驱动层：
    
    Device.cpp/Device.h（彩灯、按键、电源）

2、状态管理层：管理当前系统的状态

    manage.cpp/manage.h

3、应用逻辑层：实现具体功能

    Game.cpp/Game.h             // 游戏
    Animation.cpp/Animation.h   // 动画
    Tool.cpp/Tool.h             // 工具
    Battery.cpp/Battery.h       // 电池

4、循环调度层

    WS2812_Keychain.ino

*/

void setup() {
    Serial.begin(115200);
    EEPROM.begin();
    load_brightness_from_eeprom();
    // 初始化所有硬件驱动
    WS2812_Init();
    Key_Init();
    Voltage_Init();
}

void loop() {
    // 1. 处理用户输入，更新状态
    handle_input();

    // 2. 根据更新后的状态，渲染一帧 (现在是打印信息)
    render_frame();

    // 3. 处理后台任务 (如电压检测)
    Voltage_task();
}