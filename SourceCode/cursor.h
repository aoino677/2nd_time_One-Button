#pragma once
#include "all.h"
// カーソルの画像の定数
#define CURSOL_TEX_W			(42.0f)		// プレイヤーの画像1つの幅
#define CURSOL_TEX_H			(60.0f)		// プレイヤーの画像1つの高さ
#define CURSOL_PIVOT_X		(21.0f)    // プレイヤーの横方向の中心
#define CURSOL_PIVOT_Y      (30.0f)		// プレイヤーの縦方向の下端
#define CURSOL_SCALE			(1.0f)		// プレイヤーのスケール

void cursor_init();
void cursor_update();
void cursor_render();
void cursor_deinit();
