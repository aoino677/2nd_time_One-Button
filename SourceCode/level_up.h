#pragma once
#include "all.h"

#define LEVEL_UP_POS_X	(952.0f)
#define LEVEL_UP_POS_Y	(80.0f)
#define LEVEL_UP_SCALE	(1.1f)
#define LEVEL_UP_TEX_W	(700.0f)
#define LEVEL_UP_TEX_H	(50.0f)
#define LEVEL_UP_PIVOT_X	(350.0f)
#define LEVEL_UP_PIVOT_Y	(25.0f)

void level_up_init();
void level_up_update();
void level_up_render();
void level_up_deinit();

#define LEVEL_START (10)  // ç≈èâÇ…EXPÇéÊÇÍÇÈêî

extern int exp_count;
extern int level_max;
