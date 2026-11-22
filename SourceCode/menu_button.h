#pragma once

#include "all.h"

// ポーズボタン
#define MENU_BUTTON_TEX_W        (380)    // スタートボタンの画像1つの幅
#define MENU_BUTTON_TEX_H        (600)     // スタートボタンの画像1つの高さ
#define MENU_BUTTON_PIVOT_X      (380 * 0.5f)     // スタートボタンの横方向の中心
#define MENU_BUTTON_PIVOT_Y      (600)     // スタートボタンの縦方向の下端
#define MENU_BUTTON_SCALE        (0.8f)       // スタートボタンのスケール


void menu_button_init();
void menu_button_deinit();
void menu_button_update();
void menu_button_render();
