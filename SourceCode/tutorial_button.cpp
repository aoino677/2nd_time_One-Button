#include "tutorial_button.h"
#include "OBJ2D.h"
namespace tutorial
{
	Sprite** sprTutorial;
	int timer;
}
int tutorial_number = 0;
using namespace tutorial;

bool is_tutorial;

void tutorial_init()
{
	sprTutorial = new Sprite * [4];
	sprTutorial[0] = sprite_load(L"Data/Images/tutorial1.png");
	sprTutorial[1] = sprite_load(L"Data/Images/tutorial2.png");
	sprTutorial[2] = sprite_load(L"Data/Images/tutorial3.png");
	sprTutorial[3] = sprite_load(L"Data/Images/tutorial4.png");
	tutorial_number = 0;
}

void tutorial_deinit()
{
	safe_delete_array(sprTutorial);
}

void tutorial_update()
{
	timer++;

	if (timer > 60)
	{
		timer = 60;
		if (TRG(0) & PAD_START)
		{
			tutorial_number++;
			if (tutorial_number >= 4)
			{
				tutorial_number = 0;
				is_tutorial = false;
			}
			timer = 0;
		}
	}
}

void tutorial_render()
{
	sprite_render(
		sprTutorial[tutorial_number],
		(1 - timer / 60.0f) * SCREEN_W, 0
	);

	if (tutorial_number > 0)
	{
		sprite_render(
			sprTutorial[tutorial_number - 1],
			-(timer / 60.0f) * SCREEN_W, 0
		);
	}
}
