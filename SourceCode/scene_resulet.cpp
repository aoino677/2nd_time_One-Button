#include "all.h"
#include "result_button.h"
#include "effect_scene_change.h"

int resulet_state = 0;
int resulet_timer = 0;
Sprite* sprresult;
Sprite* sprResult_logo;

void resulet_init()
{
	resulet_state = 0;
	resulet_timer = 0;
	scene_change_init();
	result_button_init();
}

void resulet_update()
{
	switch (resulet_state)
	{
	case 0:
		resulet_state++;

		/*fallthrough*/
	case 1:
		GameLib::setBlendMode(Blender::BS_ALPHA);
		sprresult = sprite_load(L"Data/Images/result_back.png");
		sprResult_logo = sprite_load(L"Data/Images/result.png");

		resulet_state++;
		/*fallthrough*/
	case 2:
		result_button_update();
		
		break;
	}

	resulet_timer++;
}

void resulet_render()
{
	GameLib::clear(1.0f, 0.2f, 0.2f);
	font::textOut(1, "PRESS SPACE KEY", 640, 300);

	sprite_render(
		sprresult,
		0, 0
	);

	sprite_render(
		sprResult_logo,
		400, 0,
		0.8, 0.8
	);

	result_button_render();
	scene_change_render();
}

void resulet_deinit()
{
	safe_delete(sprresult);
	safe_delete(sprResult_logo);
	result_button_deinit();
}