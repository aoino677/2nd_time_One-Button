#include "all.h"
#include "EXP.h"
#include "enemy.h"

int EXP_timer = 0;
int EXP_state = 0;

Sprite* sprEXP;
OBJ2D EXP;

void EXP_init()
{
	EXP_timer = 0;
	EXP_state = 0;

}

void EXP_update()
{
	switch (EXP_state)
	{
	case 0:
		//////// èâä˙ê›íË ////////

		// EXPÇÃì«Ç›çûÇ›
		sprEXP = sprite_load(L"./Data/Images/EXP.png");

		EXP_state++;
		break;
	case 1:
		EXP = {};
		EXP.pos = { 500, 500 };
		EXP.scale   = { PAUSE_SCALE , PAUSE_SCALE };
		EXP.texSize = { PAUSE_TEX_W , PAUSE_TEX_H };
		EXP.pivot   = { PAUSE_PIVOT_X , PAUSE_PIVOT_Y };
		EXP.color   = { 1, 1, 1, 1 };

		EXP_state++;
		break;
	case 2:
		//EXP.pos = { enemy.pos.x, enemy.pos.y }


		EXP_state++;
		break;
	}

}

void EXP_render()
{
	sprite_render(
		sprEXP,
		EXP.pos.x, EXP.pos.y,
		PAUSE_SCALE, PAUSE_SCALE,
		0, 0,
		PAUSE_TEX_W, PAUSE_TEX_H,
		PAUSE_PIVOT_X, PAUSE_PIVOT_Y,
		ToRadian(0),
		1, 1, 1, 1
	);
}

void EXP_deinit()
{
	safe_delete(sprEXP);
}
