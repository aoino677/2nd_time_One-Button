#pragma once
#include "all.h"

// ポーズ画面
#define PAUSE_SCALE        (0.7f)
#define PAUSE_TEX_W        (1100.0f)
#define PAUSE_TEX_H        (200.0f)
#define PAUSE_PIVOT_X      (550.0f)
#define PAUSE_PIVOT_Y      (100.0f)

// ポーズボタン
#define PAUSEBUTTON_POS_X   (1840)
#define PAUSEBUTTON_POS_Y   (60)
#define PAUSEBUTTON_SCALE   (1.0f)
#define PAUSEBUTTON_TEX_W   (64.0f)
#define PAUSEBUTTON_TEX_H   (64.0f)
#define PAUSEBUTTON_PIVOT_X (34.0f)
#define PAUSEBUTTON_PIVOT_Y (34.0f)

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
