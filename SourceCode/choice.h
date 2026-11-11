#pragma once
#include "all.h"

#define LEVEL_COUNT_INCREAS (5) // 経験値のマックスを？？？増やす
#define CHOICE_MAX           (3)
#define CHOICE_SCALE         (1.0f)
#define CHOICE_TEX_W        (384.0f)
#define CHOICE_TEX_H         (768.0f)
#define CHOICE_PIVOT_X      (192.0f)
#define CHOICE_PIVOT_Y      (384.0f)

void choice_init();
void choice_update();
void choice_update_pause();
void choice_render();
void choice_deinit();
bool T_or_F(int level_up_count);
void randoms();

enum
{
	HP_UP,
	BULLET_SCALE_UP,
	BULLET_SPEED_UP,
	BULLET_RAD_DOWN,
	BULLET_QUANTITY_UP,
	ITEM_LUCKY,
	BULLET_INTERVAL,
	LOSER
};


