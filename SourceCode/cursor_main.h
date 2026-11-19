#pragma once
#include "all.h"
// カーソルの画像の定数
#define CURSOL_MAIN_TEX_W        (64.0f)   // プレイヤーの画像1つの幅
#define CURSOL_MAIN_TEX_H        (64.0f)   // プレイヤーの画像1つの高さ
#define CURSOL_MAIN_PIVOT_X      (34.0f)    // プレイヤーの横方向の中心
#define CURSOL_MAIN_PIVOT_Y      (34.0f)   // プレイヤーの縦方向の下端
#define CURSOL_MAIN_SCALE        (1.0f)       // プレイヤーのスケール

// 武器ごとのクールタイム
#define TIMER          (30) // Y1
#define TIMER_SHELL    (60) // Shell
#define TIMER_INGOT    (45) // Ingot
#define TIMER_BEAM     (50) // Beam
#define TIMER_BANKROLL (40) // Bankroll

extern OBJ2D cuesor_main;
extern float cursor_scale;
// 武器ごとのタイマー
extern int timer_down_bullet;
extern int timer_down_shell;
extern int timet_down_ingot;
extern int timer_down_beam;
extern int timer_down_bankroll;

extern int serect_weapon;

void cursor_main_init();
void cursor_main_update();
void cursor_main_render();
void cursor_main_deinit();
