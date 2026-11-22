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
	sprEXP_Background = sprite_load(L"Data/Images/tower.png");
}

void background_render()
{
	sprite_render(
		sprBackground,
		0+shakeX2,shakeY2
	);

	sprite_render(
		sprEXP_Background,
		0 + shakeX1, 0 + shakeY1,
		1,1,
		0,0,
		1920,1080,
		0,0,
		0,
		1,1,1
	);
}
