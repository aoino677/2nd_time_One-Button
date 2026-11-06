#include "all.h"

int farame_timer;
int second_timer;



void time_limit_init()
{
	farame_timer = 0;
	second_timer = 180;
}

void time_limit_update()
{
	farame_timer++;
	if (farame_timer > 60)
	{
		second_timer--;
		farame_timer = 0;
	}

	if(second_timer==0)
		nextScene = SCENE_RESULT;

}

void time_limit_render()
{
	debug::setString("second_timer:%d", second_timer);
}