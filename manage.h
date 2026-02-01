#ifndef _MAMAGE_H_
#define _MAMAGE_H_

#include "Device.h"
#include "Game.h"
#include "Animation.h"


void handle_input(KeyEvent event);
void render_frame(void);
void handle_input(void);
void render_frame(void);
void draw_main_menu_icon(MainMode mode);
void draw_game_icon(GameMode mode);
void draw_tool_icon(ToolMode mode);
void render_battery_overlay(void);
void draw_brightness_icon(uint8_t level);
void render_charging_display(void);
void render_charge_full_display(void);
void load_brightness_from_eeprom(void);
void save_brightness_to_eeprom(uint8_t level);
#endif