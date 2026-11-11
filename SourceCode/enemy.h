#pragma once
#include "all.h"

// --- 敵の種類を定義 ---
#define ENEMY_TYPE_CRAYON    0  //クレヨン
#define ENEMY_TYPE_BOLL      1  //ボール
#define ENEMY_TYPE_BALLOON   2  //風船
#define ENEMY_TYPE_PENCIL    3  //ロケット鉛筆
#define ENEMY_TYPE_BOBM      5  //ボム

#define BOSS_TYPE_CRAYON     6
#define BOSS_TYPE_MINICAR    7
#define BOSS_TYPE_DOLL       8
#define BOSS_TYPE_ROBOT      9
#define BOSS_TYPE_HORSE      10
#define BOSS_TYPE_PENCIL     11
#define BOSS_TYPE_EXCAVATOR  11


#define ENEMY             0
#define BOSS              1

#define ENEMY_NUMBER    (50)

#define ENEMY_SCALE     (1)
#define ENEMY_TEX_W     (128)
#define ENEMY_TEX_H     (128)
#define ENEMY_PIVOT_X   (64)
#define ENEMY_PIVOT_Y   (64)

void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();
void enemy_move(int i);
void enemy_respawn(int i);
void enemy_boss_respawn(int i);
void enemy_attack_action(int i);
void enemy_powerup();
void enemy_setting(int i, int hp, int attack, float speed);


extern OBJ2D enemy[ENEMY_NUMBER];
extern int enemy_max;