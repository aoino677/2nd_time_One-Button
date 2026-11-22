#include "level_up.h"

OBJ2D level_up;
Sprite* sprlevel_up;

int level_max;
bool TorF;
int level_up_state;

void level_up_init()
{
	exp_count = 0;
	level_max = LEVEL_START;
	level_up_state = 0;
}
void level_up_update()
{
	switch (level_up_state)
	{
	case 0:
		sprlevel_up = sprite_load(L"./Data/Images/frameEXP.png");

		level_up_state++;
		break;
	case 1:
		// パラメーターの設定
		level_up = {};
		level_up.pos			= { LEVEL_UP_POS_X , LEVEL_UP_POS_Y };
		level_up.scale		= { LEVEL_UP_SCALE , LEVEL_UP_SCALE };
		level_up.texSize		= { LEVEL_UP_TEX_W , LEVEL_UP_TEX_H };
		level_up.pivot		= { LEVEL_UP_PIVOT_X , LEVEL_UP_PIVOT_Y };
		level_up.color		= { 1.0f, 1.0f, 1.0f, 1.0f };

		break;
	}
}
void level_up_render()
{
	float rate = (float)exp_count / (float)level_max;
	if (rate > 1.0f) rate = 1.0f; 
	float levelbar_max = 700.0f * rate;

	primitive::rect(
		495.0f, 25.0f,
		levelbar_max, 50.0f
		
	);
	
	sprite_render(
		sprlevel_up,
		level_up.pos.x, level_up.pos.y,
		level_up.scale.x, level_up.scale.y,
		0, 0,
		level_up.texSize.x, level_up.texSize.y,
		level_up.pivot.x, level_up.pivot.y,
		ToRadian(0),
		level_up.color.x,
		level_up.color.y,
		level_up.color.z,
		level_up.color.w
	);
}

void level_up_deinit()
{
	safe_delete(sprlevel_up);
}

