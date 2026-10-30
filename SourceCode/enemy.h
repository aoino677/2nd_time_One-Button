#pragma once

// --- 敵の種類を定義 ---
#define ENEMY_TYPE_NORMAL 0  // 通常の敵
#define ENEMY_TYPE_TRICKY 1  // トリッキーな敵

#define ENEMY_NUMBER  (50)

#define ENEMY_SCALE      (1)
#define ENEMY_TEX_W     (128)
#define ENEMY_TEX_H      (128)
#define ENEMY_PIVOT_X   (64)
#define ENEMY_PIVOT_Y   (64)

void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();
void enemy_move(int i);
void enemy_respawn(int i);
extern OBJ2D enemy[ENEMY_NUMBER];