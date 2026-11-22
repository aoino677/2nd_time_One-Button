#pragma once
#include "all.h"

#define GRID_COLS 50 
#define GRID_ROWS 70
#define TOTAL_COINS (GRID_COLS * GRID_ROWS)

struct Coin
{
	float x, y;     // 座標
	float start_y;  // Y座標スタート地点
	float target_y; // Y座標目標地点
	float vy;       // 落下速度
	float rot;      // 回転角度
	float scale;    // スケール
	int type;       // タイプ
};

void scene_change_init();
void scene_change_deinit();
void scene_change_update();
void scene_change_render();

// どこからでも指定シーンへコイン演出つきで遷移する
void scene_change_start(int next_scene_id);

// リトライ用のコイン演出を開始する
void scene_change_start_retry();

// 演出中か／完了したかを確認する
bool is_scene_changing();
bool is_scene_change_finished();

extern bool scene_switched;