#include "power_up.h"
#include "Hp.h"

int power_up_state = 0;
int power_up_timer = 0;

OBJ2D efficacy[EFFICACY_MAX];
Sprite* sprefficacy;

void power_up_init()
{
	power_up_state = 0;
	power_up_timer = 0;
}

void power_up_update()
{
	switch (power_up_state)
	{
	case 0:

		power_up_state++;
		break;
	case 1:

		power_up_state++;
		break;

	case 2:

		break;
	}
}

void power_up_render()
{
}

void power_up_deinit()
{
}

void Rapidfire()
{
	
}

void power()
{

}

void HP()
{
	player_hp += 3;
}
