#include "all.h"

int level_up_count;
extern int exp_count;
int level_max;

void level_up_init()
{
	level_up_count = 0;
	exp_count = 0;
	level_max = 10;
}

void level_up_update()
{
	if (exp_count >= level_max)
	{
		level_up_count++;
		level_max += 5;
		exp_count = 0;
	}
}

void level_up_render()
{
	float rate = (float)exp_count / (float)level_max;
	if (rate > 1.0f) rate = 1.0f; 
	float barWidth = 750.0f * rate;

	primitive::rect(
		530.0f, 50.0f,
		barWidth, 100.0f
		
	);

	debug::setString("exp_count:%d", exp_count);
}