#pragma once

#define EXP_COUNT      (256)      // EXP‚ÌŒÂ”

#define EXP_SCALE        (0.5f)
#define EXP_TEX_W        (64.0f)
#define EXP_TEX_H        (64.0f)
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