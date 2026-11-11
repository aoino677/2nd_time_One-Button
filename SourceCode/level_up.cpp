#include "level_up.h"

int level_max;
bool TorF;

void level_up_init()
{
	exp_count = 0;
	level_max = LEVEL_START;
}
void level_up_render()
{
	float rate = (float)exp_count / (float)level_max;
	if (rate > 1.0f) rate = 1.0f; 
	float levelbar_max = 750.0f * rate;

	primitive::rect(
		530.0f, 50.0f,
		levelbar_max, 100.0f
		
	);

	debug::setString("exp_count:%d", exp_count);
}

