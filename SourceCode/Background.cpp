#include "Background.h"
#include "shake.h"

Sprite* sprBackground;
Sprite* sprEXP_Background;


void background_deinit()
{
	safe_delete(sprBackground);
	safe_delete(sprEXP_Background);
}

void background_init()
{
	sprBackground = sprite_load(L"Data/Images/BackGround.png");
	sprEXP_Background = sprite_load(L"Data/Images/EXP_BackGround.png");
}

void background_render()
{
	sprite_render(
		sprBackground,
		0+shakeX, 0+shakeY
	);

	sprite_render(
		sprEXP_Background,
		-100, 0
	);
}
