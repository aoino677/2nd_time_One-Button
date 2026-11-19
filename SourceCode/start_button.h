#pragma once
#include "all.h"

// ポーズボタン
#define TITLE_BUTTON_TEX_W        (1100.0f)    // スタートボタンの画像1つの幅
#define TITLE_BUTTON_TEX_H        (500.0f)     // スタートボタンの画像1つの高さ
#define TITLE_BUTTON_PIVOT_X      (550.0f)     // スタートボタンの横方向の中心
#define TITLE_BUTTON_PIVOT_Y      (100.0f)     // スタートボタンの縦方向の下端
#define TITLE_BUTTON_SCALE        (0.6f)       // スタートボタンのスケール


void title_button_init();
void title_button_deinit();
void title_button_update();
void title_button_render();

