#pragma once
#include "all.h"

//通常敵
#define ENEMY_TYPE_NONE      -1
#define ENEMY_TYPE_CRAYON    0  //クレヨン
#define ENEMY_TYPE_BOLL      1  //ボール
#define ENEMY_TYPE_BALLOON   2  //風船
#define ENEMY_TYPE_ROCKET    3  //ロケット鉛筆
#define ENEMY_TYPE_BOBM      5  //ボム

//ボス敵
#define BOSS_TYPE_CRAYON     6
#define BOSS_TYPE_CAR        7
#define BOSS_TYPE_DOLL       8
#define BOSS_TYPE_ROBOT      9
#define BOSS_TYPE_HORSE      10
#define BOSS_TYPE_PENCIL     11
#define BOSS_TYPE_EXCAVATOR  11

#define ENEMY             0
#define BOSS              1

#define ENEMY_NUMBER    (100)

#define PARTICLE_NUMBER 150 // 画面全体で同時に扱える破片の最大数

#define ENEMY_SCALE     (1)
#define ENEMY_TEX_W     (128)
#define ENEMY_TEX_H     (160)
#define ENEMY_PIVOT_X   (64)
#define ENEMY_PIVOT_Y   (64)

void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();
void enemy_setting(int i, int hp, int attack, float speed, float a, float b, float x, float y, float n, float m,int z);
void enemy_respawn(int i);
void enemy_crayon_respawn();
void enemy_boss_respawn(int i);
void enemy_move(int i);
void enemy_attack_action(int i);
void enemy_powerup();
void enemy_damage_effect(int i);
void enemy_knockback_check(int i);
void enemy_bomb_explosion(int i);
void spawn_enemy_particles(float x, float y, float r, float g, float b);
void particle_update_and_render();

void enemy_car_action(int i);
void effect_boss_respawn();


struct PARTICLE {
	bool active;      // 使用中かどうか
	float x, y;       // 座標
	float vx, vy;     // 速度（移動方向とスピード）
	float size;       // ブロックの大きさ
	float alpha;      // 透明度
	int life;         // 残り寿命
	int max_life;     // 全体の寿命
	float r, g, b;    // 色（赤・緑・青）
};

extern OBJ2D enemy[ENEMY_NUMBER];
extern int enemy_max;