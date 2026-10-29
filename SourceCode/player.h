#pragma once
#include "all.h"

struct Player
{
	VECTOR2 pos;
	VECTOR2 texSize;
};

extern OBJ2D player;

const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
void player_init();
void player_deinit();
void player_update();
void player_render();

void set_canRapidShot(bool);
void set_shotPerSecond(float);

