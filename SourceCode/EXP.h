#pragma once
#include "all.h"

#define EXP_COUNT      (256)      // EXP‚ÌŒÂ”

#define EXP_SCALE        (0.4f)
#define EXP_TEX_W        (64.0f)
#define EXP_TEX_H        (64.0f)
#define EXP_PIVOT_X      (32.0f)
#define EXP_PIVOT_Y      (32.0f)


void EXP_init();
void EXP_update();
void EXP_render();
void EXP_deinit();

// externéŒ¾
extern Sprite* sprEXP;
extern OBJ2D EXP[EXP_COUNT];
extern float acquisition_range;