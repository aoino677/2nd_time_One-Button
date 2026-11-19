#pragma once
#include "all.h"
// •Ší‚ÌƒŒƒxƒ‹
enum WEAPON_LEVEL
{
	level_1 = 1,
	level_2,
	level_3,
	level_4,
	level_5,
	level_6,
	level_7,
	level_8
};

// •Ší‚Ìí—Ş
enum WEAPON_TYPE
{
	Y1 = 0,
	SHELL,
	WEAPON_3,
	WEAPON_4,
	WEAPON_5,
	WEAPON_MAX
};

extern int level_up_weapon[WEAPON_MAX];
extern float Y1_attack_range;

void level_init();
void level_update();