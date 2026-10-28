#pragma once
#include "all.h"

// ポーズ画面
#define PAUSE_SCALE        (0.7f)
#define PAUSE_TEX_W        (1100.0f)
#define PAUSE_TEX_H        (200.0f)
#define PAUSE_PIVOT_X      (550.0f)
#define PAUSE_PIVOT_Y      (100.0f)

// ポーズボタン
#define PAUSEBUTTON_POS_X   (SCREEN_W - 95.0f)
#define PAUSEBUTTON_POS_Y   (SCREEN_H - 55.0f)
#define PAUSEBUTTON_SCALE   (1.0f)
#define PAUSEBUTTON_TEX_W   (150.0f)
#define PAUSEBUTTON_TEX_H   (70.0f)
#define PAUSEBUTTON_PIVOT_X (75.0f)
#define PAUSEBUTTON_PIVOT_Y (35.0f)

extern int pause_state;
extern int pause_timer;

extern OBJ2D pauseButton[3];
extern OBJ2D pauseIcon;
extern Sprite* sprPauseButton[3];
extern Sprite* sprPauseIcon;

void pause_init();
void pause_deinit();
void pause_update();
void pause_render();
