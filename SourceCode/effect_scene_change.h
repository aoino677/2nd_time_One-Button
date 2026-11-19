#pragma once
#include "all.h"

#define GRID_COLS 70 
#define GRID_ROWS 90
#define TOTAL_COINS (GRID_COLS * GRID_ROWS)


struct Coin 
{
	float x, y;     //座標
	float start_y;  //Y座標スタート地点
	float target_y; //Y座標目標地点
	float vy;       //落下速度
	float rot;      //回転角度
	float scale;    //スケール
	int type;       //タイプ
};

void scene_change_update();
void scene_change_init();
void scene_change_deinit();
void scene_change_render();

extern bool scene_switched;
bool is_scene_change_finished();
