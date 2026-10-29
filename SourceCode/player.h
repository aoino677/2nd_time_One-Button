#pragma once
#include "all.h"

struct Bullet
{
    VECTOR2 pos;//åªç›à íu
    VECTOR2 velocity;//ë¨ìx
};

struct Player
{
	VECTOR2 pos;
};

extern OBJ2D player;

const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
void player_init();
void player_deinit();
void player_update();
void player_render();

void set_canRapidShot(bool);
void set_shotPerSecond(int);

