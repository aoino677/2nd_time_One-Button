#include "Hp.h"
#include "result_screen.h"

OBJ2D hp;
Sprite* sprhp;

int player_hp;
int max_hp;
int hp_state;

void hp_init()
{
<<<<<<< HEAD
	player_hp = 100;
	max_hp = 100;
=======
	player_hp = 10;
	max_hp = 10;
	hp_state = 0;
}

void hp_update()
{
	switch (hp_state)
	{
	case 0:
		sprhp = sprite_load(L"./Data/Images/frameHP.png");

		hp_state++;
		break;
	case 1:
		// パラメーターの設定
		hp = {};
		hp.pos		= { HP_POS_X ,		 HP_POS_Y };
		hp.scale		= { HP_SCALE,		 HP_SCALE };
		hp.texSize	= { HP_TEX_W ,	 HP_TEX_H };
		hp.pivot		= { HP_PIVOT_X ,	 HP_PIVOT_Y };
		hp.color		= { 1.0f, 1.0f, 1.0f, 1.0f };

		//level_up_state++;
		break;
	}
>>>>>>> 4201f7e03b25e8b27c669a98c2c476a1ac490436
}


void hp_update()
{
	if (player_hp <= 0)
	{
		player_hp = 0;
		result_active = true;
	}
}

void hp_render()
{
	float hp_rate = (float)player_hp / (float)max_hp;
	if (hp_rate < 0.0f) hp_rate = 0.0f;
	if (hp_rate > 1.0f) hp_rate = 1.0f;
	
	float hp_bar_max = 440.0f * hp_rate;

	primitive::rect(
	    1375, 25,
		hp_bar_max, 100,
		0, 0,
		0,
		1, 0, 0);

	sprite_render(
		sprhp,
		hp.pos.x, hp.pos.y,
		hp.scale.x, hp.scale.y,
		0, 0,
		hp.texSize.x, hp.texSize.y,
		hp.pivot.x, hp.pivot.y,
		ToRadian(0),
		hp.color.x,
		hp.color.y,
		hp.color.z,
		hp.color.w
	);

	debug::setString("player_hp:%d", player_hp);
}

void hp_deinit()
{
	safe_delete(sprhp);
}
