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

struct Bullet_Y1_AfterImage
{
	Bullet_Y1_AfterImage(const VECTOR2&, const float, const int);
	VECTOR2 pos;
	float angle = 0;
	int color_index;
	float timer = 0;
};

void set_vector_Bullet_Y1_AfterImage(Bullet_Y1_AfterImage* image);
//extern int bullet_ingot_counter;

//const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H * 0.5f };
//const VECTOR2 PLAYER_POS = { SCREEN_W * 0.5f, SCREEN_H - 100 };
const VECTOR2 PLAYER_POS = { 30,400  };
const VECTOR2 PLAYER_TEX_SIZE = { 128, 128 };

void player_init();
void player_deinit();
void player_update();
void player_render();

//void set_canRapidShot(bool);
void set_shotPerSecond(float);

void bullet_update();

void shot_bullet();
void shot_bullet_Ingot();
void shot_bullet_Shell();

const float PLAYER_EASING = 0.99f;

//#define RAPID_COOLDOWN (0.2f)
#define BULLET_SPEED (4000.0f)

//ÇPâ~
#define Y1_DAMAGE (8)

//äLäk
#define SHELL_DAMAGE (30)//Y1_damageÇÃî{ó¶Ç‡èÊÇ¡ÇƒÇ¢Ç‹Ç∑ÅB

//âÑÇ◊ñ_
#define INGOT_DAMAGE (18)//Y1_damageÇÃî{ó¶Ç‡èÊÇ¡ÇƒÇ¢Ç‹Ç∑ÅB
