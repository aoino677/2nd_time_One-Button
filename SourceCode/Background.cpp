#include "all.h"

Sprite* sprBackground;
extern int shakeTimer;
extern float shakePower;
extern float shakeX;
extern float shakeY;

void background_deinit()
{
	safe_delete(sprBackground);
}

void background_init()
{
	sprBackground = sprite_load(L"Data/Images/BackGround.png");
}

void background_render()
{
	sprite_render(
		sprBackground,
		0+shakeX, 0+shakeY
	);
}
