#include "Hp.h"
#include "result_screen.h"

int player_hp;
int max_hp;

void hp_init()
{
	player_hp = 100;
	max_hp = 100;
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
	
	float hp_bar_max = 500.0f * hp_rate;

	primitive::rect(
	    1400, 50,
		hp_bar_max, 100,
		0, 0,
		0,
		1, 0, 0);

	debug::setString("player_hp:%d", player_hp);
}