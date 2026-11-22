#pragma once

#include "all.h"

// ポーズボタン
#define MENU_BUTTON_TEX_W        (380)    // スタートボタンの画像1つの幅
#define MENU_BUTTON_TEX_H        (600)     // スタートボタンの画像1つの高さ
#define MENU_BUTTON_PIVOT_X      (380 * 0.5f)     // スタートボタンの横方向の中心
#define MENU_BUTTON_PIVOT_Y      (600)     // スタートボタンの縦方向の下端
#define MENU_BUTTON_SCALE        (0.8f)       // スタートボタンのスケール

#define MENU_TEXT_POS_X (SCREEN_W * 0.5f)
#define MENU_TEXT_POS_Y (SCREEN_H * 0.2f)
#define MENU_TEXT_SCALE_X (0.5f)
#define MENU_TEXT_SCALE_Y (0.5f)
#define MENU_TEXT_TEX_W (1500)
#define MENU_TEXT_TEX_H  (200)

void menu_button_init();
void menu_button_deinit();
void menu_button_update();
void menu_button_render();
