#pragma once
#include "all.h"

#define ENEMY_NUMBER     (100)

//通常敵
#define ENEMY_TYPE_NONE      -1 //なし
#define ENEMY_TYPE_CRAYON    0  //クレヨン
#define ENEMY_TYPE_BOLL      1  //ボール
#define ENEMY_TYPE_BALLOON   2  //風船
#define ENEMY_TYPE_ROCKET    3  //ロケット鉛筆
#define ENEMY_TYPE_BOBM      4  //ボム

//ボス敵
#define BOSS_TYPE_CRAYON     5
#define BOSS_TYPE_CAR        6
#define BOSS_TYPE_DOLL       7
#define BOSS_TYPE_ROBOT      8
#define BOSS_TYPE_HORSE      9
#define BOSS_TYPE_PENCIL     10
#define BOSS_TYPE_EXCAVATOR  11

#define ENEMY             0
#define BOSS              1

#define PARTICLE_NUMBER 150 

#define TIRE_PARTICLE_NUMBER 4


#define ENEMY_SCALE     (1)
#define ENEMY_TEX_W     (128)
#define ENEMY_TEX_H     (160)
#define ENEMY_PIVOT_X   (64)
#define ENEMY_PIVOT_Y   (64)

//タイヤ構造体
struct TireEffect {
	bool active;
	float x, y;
	float vx, vy;
	float rot;
	float rot_speed;
	Sprite* sprite; // 前輪か後輪の画像
};

//死亡エフェクト構造体
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

//基本関数
void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();

//リスポーン関数
void enemy_setting(int i, int hp, int attack, float speed, float a, float b, float x, float y, float n, float m, int z);
void enemy_crayon_respawn();
void enemy_respawn(int i);
void enemy_boss_respawn(int i);

//アビリティ関数
void enemy_move(int i);
void enemy_bomb_explosion(int i);
void enemy_attack_action(int i);
void enemy_powerup();
void enemy_knockback_check(int i);

//エフェクト関数
void enemy_damage_effect(int i);
void spawn_enemy_particles(float x, float y, float r, float g, float b);
void particle_render();
void effect_boss_respawn();
void effect_boss_render();
void tires_update();
void tires_render();
void tires_move(float boss_x, float boss_y);

//外部参照変数
extern OBJ2D enemy[ENEMY_NUMBER];
extern int enemy_max;
extern float enemy_speed_up;