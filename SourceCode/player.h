#pragma once
#include "all.h"

struct Player
{
	VECTOR2 pos;
	VECTOR2 texSize;
};

extern OBJ2D player;

const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
const VECTOR2 PLAYER_TEX_SIZE = { 128, 128 };

void player_init();
void player_deinit();
void player_update();
void player_render();

void set_canRapidShot(bool);
void set_shotPerSecond(float);
void set_bullet_flavor(int);

#define RAPID_COOLDOWN (0.2f)
