#pragma once
#include "all.h"

// --- “G‚Ìí—Ş‚ğ’è‹` ---
#define ENEMY_TYPE_NORMAL 0  //’Êí‚Ì“G
#define ENEMY_TYPE_FAST   1  //‘¬‚¢“G
#define ENEMY_TYPE_TURBO  2  //ˆê’è‚ÌŠÔŠu‚Åi‚Ş“G
#define ENEMY_TYPE_TRICKY 3  //ƒgƒŠƒbƒL[‚È“G
#define ENEMY_TYPE_EDDY   4  //‰QŠª‚­“G
#define ENEMY_TYPE_BOBM   5  //ƒ{ƒ€“G
#define ENEMY_TYPE_BOSS   6  //ƒ{ƒX“G

#define ENEMY_NUMBER  (50)

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
void enemy_attack_action(int i);
void enemy_powerup();

void CRAYON(int i,int attack,float speed);
void BOBM(int i, int hp, int attack, float speed);

extern OBJ2D enemy[ENEMY_NUMBER];
extern int enemy_max;