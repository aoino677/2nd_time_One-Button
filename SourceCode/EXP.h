#pragma once

#define EXP_COUNT      (50)      // EXP‚ÌŒÂ”

#define EXP_SCALE        (1.0f)
#define EXP_TEX_W        (32.0f)
#define EXP_TEX_H        (32.0f)
#define EXP_PIVOT_X      (16.0f)
#define EXP_PIVOT_Y      (16.0f)


void EXP_init();
void EXP_update();
void EXP_render();
void EXP_deinit();

// EXP‚ªƒvƒŒƒCƒ„[‚É‹ß‚Ã‚­
bool move_block_player(OBJ2D* EXP, OBJ2D* Player, float speed);

// externéŒ¾
extern Sprite* sprEXP;
extern OBJ2D EXP[EXP_COUNT];