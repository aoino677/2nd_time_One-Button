#pragma once

#define EXP_SCALE        (1.0f)
#define EXP_TEX_W        (32.0f)
#define EXP_TEX_H        (32.0f)
#define EXP_PIVOT_X      (16.0f)
#define EXP_PIVOT_Y      (16.0f)


void EXP_init();
void EXP_update();
void EXP_render();
void EXP_deinit();

bool move_block_player(OBJ2D* EXP, OBJ2D* Player, float speed);