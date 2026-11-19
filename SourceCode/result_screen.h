#pragma once

#pragma once
#include "all.h"

void result_screen_init();
void result_screen_deinit();
void result_screen_update();
void result_screen_render();
bool result_screen_is_finished();

// ボタン
#define SCREEN_POS_X      384.0f  // X座標
#define SCREEN_POS_Y      -756.0f // 開始Y座標
#define SCREEN_STOP_Y     162.0f  // 停止Y座標
#define SCREEN_RESULT_W   1920.0f // 横幅
#define SCREEN_RESULT_H   1080.0f  // 縦幅
#define SCREEN_SPEED      30.0f   // 落下スピード

extern bool result_active;
