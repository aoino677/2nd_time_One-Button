#pragma once

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
void enemy_walk();
void enemy_position();

extern OBJ2D enemy[ENEMY_NUMBER];