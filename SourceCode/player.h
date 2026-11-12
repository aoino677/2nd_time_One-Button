#pragma once
#include "all.h"

struct Player
{
	VECTOR2 pos;
	VECTOR2 texSize;
};

extern OBJ2D player;
extern float bullet_speed;
extern int bullet_lay_count;
extern float bullet_lay_gap;
extern float bullet_scale;
extern float precision_rad;
extern float Y1_damage;
//extern int bullet_ingot_counter;

//const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
//const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H - 100 };
const VECTOR2 PLAYER_POS = { 30,400  };
const VECTOR2 PLAYER_TEX_SIZE = { 128, 128 };

void player_init();
void player_deinit();
void player_update();
void player_render();

void set_canRapidShot(bool);
void set_shotPerSecond(float);

void bullet_update();

extern void shot_bullet();

#define RAPID_COOLDOWN (0.2f)
#define BULLET_SPEED (4000.0f)

//‚P‰~
#define Y1_DAMAGE (8)

//‰„‚×–_
#define INGOT_DAMAGE (18)