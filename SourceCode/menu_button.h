#pragma once

#include "all.h"

// ポーズボタン
#define MENU_BUTTON_TEX_W        (1000.0f)    // スタートボタンの画像1つの幅
#define MENU_BUTTON_TEX_H        (200.0f)     // スタートボタンの画像1つの高さ
#define MENU_BUTTON_PIVOT_X      (500.0f)     // スタートボタンの横方向の中心
#define MENU_BUTTON_PIVOT_Y      (100.0f)     // スタートボタンの縦方向の下端
#define MENU_BUTTON_SCALE        (0.6f)       // スタートボタンのスケール


void menu_button_init();
void menu_button_deinit();
void menu_button_update();
void menu_button_render();
