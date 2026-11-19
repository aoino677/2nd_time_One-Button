#pragma once
#include "all.h"

#define LEVEL_COUNT_INCREAS (5) // 経験値のマックスを？？？増やす
#define CHOICE_MAX           (3)
#define CHOICE_SCALE         (1.0f)
#define CHOICE_TEX_W        (384.0f)
#define CHOICE_TEX_H         (600.0f)
#define CHOICE_PIVOT_X      (192.0f)
#define CHOICE_PIVOT_Y      (300.0f)
#define BUTTON_Y			   (600.0f)


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
	WEAPON_SHELL,
	WEAPON_BEAM,
	WEAPON_INGOT,
	WEAPON_BANKROLL,
	ITEM_LUCKY,
	WEAPON_BULLET,
	BULLET_DAMAGE,
	LOST_ITEM_ACQUISITION_RANGE,
	CURSOR_BIG
};


