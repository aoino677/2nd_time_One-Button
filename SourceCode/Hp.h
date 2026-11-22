#pragma once
#include "all.h"

#define HP_POS_X		(1540.0f)
#define HP_POS_Y		(75.0f)
#define HP_SCALE		(1.1f)
#define HP_TEX_W	(500.0f)
#define HP_TEX_H		(100.0f)
#define HP_PIVOT_X	(250.0f)
#define HP_PIVOT_Y	(50.0f)

extern int player_hp;
extern int max_hp;

void hp_init();
void hp_update();
void hp_render();
<<<<<<< HEAD

extern int player_hp;
=======
void hp_deinit();
>>>>>>> 4201f7e03b25e8b27c669a98c2c476a1ac490436
