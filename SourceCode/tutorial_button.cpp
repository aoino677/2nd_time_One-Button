#include "tutorial_button.h"
#include "OBJ2D.h"

OBJ2D tutorial_button;
Sprite* sprButton;


void tutorial_button_init()
{
	tutorial_button = {};
	tutorial_button.pos = { static_cast<float>(SCREEN_W) * 0.5f,static_cast<float>(SCREEN_H) * 0.8f };
	tutorial_button.texSize = { TUTORIAL_BUTTON_TEX_SIZE_W , TUTORIAL_BUTTON_TEX_SIZE_H };
	tutorial_button.pivot = { TUTORIAL_BUTTON_PIVOT_X, TUTORIAL_BUTTON_PIVOT_Y };
	tutorial_button.scale = { TUTORIAL_BUTTON_SCALE_X, TUTORIAL_BUTTON_SCALE_Y };

	sprButton = sprite_load(L"Data/Images/tutorial_sin.png");
}

void tutorial_button_deinit()
{
	safe_delete(sprButton);
}

void tutorial_button_update()
{
	auto mouseX{ static_cast<float>(getCursorPosX()) };
	auto mouseY{ static_cast<float>(getCursorPosY()) };

	float left =
		tutorial_button.pos.x -
		tutorial_button.pivot.x * tutorial_button.scale.x;

	float right =
		tutorial_button.pos.x +
		(tutorial_button.texSize.x - tutorial_button.pivot.x) * tutorial_button.scale.x;

	float top =
		tutorial_button.pos.y -
		tutorial_button.pivot.y * tutorial_button.scale.y;

	float bottom =
		tutorial_button.pos.y +
		(tutorial_button.texSize.y - tutorial_button.pivot.y) * tutorial_button.scale.y;

	if (mouseX >= left &&
		mouseX <= right &&
		mouseY >= top &&
		mouseY <= bottom)
	{
		tutorial_button.scale.x = static_cast<float>(TUTORIAL_BUTTON_SCALE_X) + 0.1f;
		tutorial_button.scale.y = static_cast<float>(TUTORIAL_BUTTON_SCALE_Y) + 0.1f;
	}
	else
	{
		tutorial_button.scale.x = static_cast<float>(TUTORIAL_BUTTON_SCALE_X);
		tutorial_button.scale.y = static_cast<float>(TUTORIAL_BUTTON_SCALE_Y);
	}
}

void tutorial_button_render()
{
	sprite_render(
		sprButton,
		tutorial_button.pos.x, tutorial_button.pos.y,
		tutorial_button.scale.x, tutorial_button.scale.y,
		tutorial_button.texPos.x, tutorial_button.texPos.y,
		tutorial_button.texSize.x, tutorial_button.texSize.y,
		tutorial_button.pivot.x, tutorial_button.pivot.y,
		tutorial_button.angle
	);
}
