#include "all.h"
#include "levelup.h"


int levelup_count;
int levelup;
int level = 5;

void levelup()
{
	if (levelup_count >=LEVEL+level )
	{
		level += 5;
		levelup_count = 0;
	}
}
