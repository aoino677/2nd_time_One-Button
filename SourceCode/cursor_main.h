#pragma once
#include "all.h"
// カーソルの画像の定数
#define CURSOL_MAIN_TEX_W        (64.0f)   // プレイヤーの画像1つの幅
#define CURSOL_MAIN_TEX_H        (64.0f)   // プレイヤーの画像1つの高さ
#define CURSOL_MAIN_PIVOT_X      (34.0f)    // プレイヤーの横方向の中心
#define CURSOL_MAIN_PIVOT_Y      (34.0f)   // プレイヤーの縦方向の下端
#define CURSOL_MAIN_SCALE        (1.0f)       // プレイヤーのスケール

#define TIMER (60)

extern OBJ2D cuesor_main;
extern float cursor_scale;

void cursor_main_init();
void cursor_main_update();
void cursor_main_render();
void cursor_main_deinit();
