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
extern float shell_damage;
extern float shell_scale;
extern float ingot_damage;
extern float ingot_scale;
extern float beem_damage;
extern float beem_scale;
extern float bankrool_damage;
extern float bankrool_scale;

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

//弾を打つ関数===========================================
void shot_bullet();
void shot_bullet_Ingot();
void shot_bullet_Shell();
void shot_bullet_Beam();
void shot_bullet_Bankroll();
void shot_bullet_Special();
//=======================================================

const float PLAYER_EASING = 0.99f;

#define BULLET_SCALE (1.0f)

//#define RAPID_COOLDOWN (0.2f)

//１円===================================================
#define Y1_DAMAGE (1)
#define BULLET_SPEED (4000.0f)

//貝殻===================================================
#define SHELL_DAMAGE (1)
#define SHELL_SPEED (2000.0f)//1秒間当たりの進むスピード

//延べ棒=================================================
#define INGOT_DAMAGE (18)//Y1_damageの倍率は乗らなくなりました。
#define INGOT_SPEED (100.0f)//1秒間当たりの進むスピード
#define INGOT_ACCEL (100.0f)//1秒間当たりの加速度

//一閃===================================================
#define BEEM_DAMAGE (18)//Y1_damageの倍率は乗らなくなりました。
#define BEEM_SPEED (4000.0f)//1秒間当たりの進むスピード
#define BEEM_ACCEL (10.0f)//1秒間当たりの加速度

//スペシャル技===========================================
#define CAN_SHOT_SPECIAL_COUNT 3
#define SPECIAL_DAMAGE (30)
#define SPECIAL_RANGE (200.0f)
const VECTOR3 bullet_special_color = { 1.0f, 0.0f, 0.0f };