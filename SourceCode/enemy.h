#pragma once

#include "all.h"

//--------------------------------------
// 定数・種別定義（#define）
//--------------------------------------
#define ENEMY_NUMBER         (32)   // 敵の最大数（必要に応じて調整）
#define TIRE_PARTICLE_NUMBER (4)    // タイヤエフェクト数

// 敵タイプ
#define ENEMY_TYPE_NONE      (0)
#define ENEMY_TYPE_BOLL      (1)
#define ENEMY_TYPE_BALLOON   (2)
#define ENEMY_TYPE_ROCKET    (3)
#define ENEMY_TYPE_BOMB      (4)
#define ENEMY_TYPE_CRAYON    (5)
#define BOSS_TYPE_BEAR       (6)
#define BOSS_TYPE_CRAYON     (7)
#define ENEMY_TYPE_SOLDIER    8
#define BOSS_TYPE_CAR        (9)
#define BOSS_TYPE_ROBOT      (10)
#define BOSS_TYPE_HORSE      (11)
#define BOSS_TYPE_PENCIL     (12)
#define BOSS_TYPE_EXCAVATOR  (13)

// 描画種別（通常敵 / ボス）
#define ENEMY                (0)
#define BOSS                 (1)

//--------------------------------------
// 構造体定義
//--------------------------------------
// タイヤ破片エフェクト構造体
struct TireEffect
{
	bool active = false;
	float x = 0.0f;
	float y = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
	float rot = 0.0f;
	float rot_speed = 0.0f;
	Sprite* sprite = nullptr;
};

//--------------------------------------
// 外部変数（他ファイルから参照・変更）
//--------------------------------------
extern int difficulty; // 0:EASY, 1:NORMAL, 2:HARD
extern int enemy_kill_count;
extern int enemy_max;
extern float  hit_buck;
extern OBJ2D enemy[ENEMY_NUMBER];
extern TireEffect tire_particles[TIRE_PARTICLE_NUMBER];
extern int enemy_max_hp[ENEMY_NUMBER];
extern int enemy_type_render[ENEMY_NUMBER];

//--------------------------------------
// 関数宣言
//--------------------------------------
void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();

void get_enemy_status_by_difficulty(int type, int& hp, float& speed, int& attack);
int roll_enemy_type_by_difficulty();

void enemy_setting(int i, float a, float b, float x, float y, float n, float m, int z);
void enemy_crayon_respawn();
void enemy_respawn(int i);
void enemy_boss_respawn(int i);
void enemy_move(int i);
void enemy_attack_action(int i);
void enemy_powerup();
void enemy_knockback_check(int i);
void enemy_bomb_explosion(int i);
void tires_move(float boss_x, float boss_y);
void effect_boss_respawn();
void enemy_damage_effect(int i);
void tires_render();
void effect_boss_render();
void set_random_crayon_color(int idx);


#define ENEMY_SCALE     (1)
#define ENEMY_TEX_W     (128)
#define ENEMY_TEX_H     (160)
#define ENEMY_PIVOT_X   (64)
#define ENEMY_PIVOT_Y   (64)