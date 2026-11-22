#include "tutorial_button.h"
#include "OBJ2D.h"
#include "button_se.h"

namespace tutorial
{
	Sprite** sprTutorial;
	Sprite* sprPrev;
	OBJ2D prevButton;
	Sprite* sprNext;
	OBJ2D nextButton;
	int timer;
}
int tutorial_number = 0;
int prev_tutorial_number = 0;
using namespace tutorial;

bool is_tutorial;

void tutorial_prev_button_update();
void tutorial_next_button_update();

void tutorial_init()
{
	sprTutorial = new Sprite * [TUTORIAL_COUNT];
	sprTutorial[0] = sprite_load(L"Data/Images/tutorial1.png");
	sprTutorial[1] = sprite_load(L"Data/Images/tutorial2.png");
	sprTutorial[2] = sprite_load(L"Data/Images/tutorial3.png");
	sprTutorial[3] = sprite_load(L"Data/Images/tutorial4.png");
	sprTutorial[4] = sprite_load(L"Data/Images/tutorial5.png");
	sprTutorial[5] = sprite_load(L"Data/Images/tutorial6.png");
	sprPrev = sprite_load(L"Data/Images/prev_page.png");
	prevButton = {};
	prevButton.pos = { TUTORIAL_BUTTON_PREV_PAGE_POS_X, TUTORIAL_BUTTON_PREV_PAGE_POS_Y };
	prevButton.scale = { TUTORIAL_BUTTON_PREV_PAGE_SCALE_X, TUTORIAL_BUTTON_PREV_PAGE_SCALE_Y };
	prevButton.texSize = { TUTORIAL_BUTTON_PREV_PAGE_TEX_SIZE_X, TUTORIAL_BUTTON_PREV_PAGE_TEX_SIZE_Y };
	prevButton.pivot = { TUTORIAL_BUTTON_PREV_PAGE_PIVOT_X, TUTORIAL_BUTTON_PREV_PAGE_PIVOT_Y };
	sprNext = sprite_load(L"Data/Images/next_page.png");
	nextButton = {};
	nextButton.pos = { TUTORIAL_BUTTON_NEXT_PAGE_POS_X, TUTORIAL_BUTTON_NEXT_PAGE_POS_Y };
	nextButton.scale = { TUTORIAL_BUTTON_NEXT_PAGE_SCALE_X, TUTORIAL_BUTTON_NEXT_PAGE_SCALE_Y };
	nextButton.texSize = { TUTORIAL_BUTTON_NEXT_PAGE_TEX_SIZE_X, TUTORIAL_BUTTON_NEXT_PAGE_TEX_SIZE_Y };
	nextButton.pivot = { TUTORIAL_BUTTON_NEXT_PAGE_PIVOT_X, TUTORIAL_BUTTON_NEXT_PAGE_PIVOT_Y };
	tutorial_number = 0;
	prev_tutorial_number = -1;
}

void tutorial_deinit()
{
	safe_delete_array(sprTutorial);
	safe_delete(sprPrev);
	safe_delete(sprNext);
}

void tutorial_update()
{
	timer++;

	if (timer > TUTORIAL_SLIDE_FLAME)
	{
		timer = TUTORIAL_SLIDE_FLAME;
	}

	tutorial_prev_button_update();
	tutorial_next_button_update();
}

void tutorial_render()
{
	sprite_render(
		sprTutorial[tutorial_number],
		(1 - timer / TUTORIAL_SLIDE_FLAME) * SCREEN_W * (tutorial_number - prev_tutorial_number), 0
	);

	if (prev_tutorial_number >= 0)
	{
		sprite_render(
			sprTutorial[prev_tutorial_number],
			(1 - timer / TUTORIAL_SLIDE_FLAME) * SCREEN_W * (tutorial_number - prev_tutorial_number) + SCREEN_W * (prev_tutorial_number - tutorial_number), 0
		);
	}

	sprite_render(
		sprPrev,
		prevButton.pos.x,
		prevButton.pos.y,
		prevButton.scale.x,
		prevButton.scale.y,
		prevButton.texPos.x,
		prevButton.texPos.y,
		prevButton.texSize.x,
		prevButton.texSize.y,
		prevButton.pivot.x,
		prevButton.pivot.y
	);

	sprite_render(
		sprNext,
		nextButton.pos.x,
		nextButton.pos.y,
		nextButton.scale.x,
		nextButton.scale.y,
		nextButton.texPos.x,
		nextButton.texPos.y,
		nextButton.texSize.x,
		nextButton.texSize.y,
		nextButton.pivot.x,
		nextButton.pivot.y
	);
}

void tutorial_prev_button_update()
{
	//----------------------------------
	//    マウス座標
	//----------------------------------
	float mouseX = static_cast<float>(getCursorPosX());
	float mouseY = static_cast<float>(getCursorPosY());

	//----------------------------------
	//    ボタン当たり判定
	//----------------------------------
	float left =
		prevButton.pos.x -
		prevButton.pivot.x * prevButton.scale.x;

	float right =
		left + prevButton.texSize.x * prevButton.scale.x;

	float top =
		prevButton.pos.y -
		prevButton.pivot.y * prevButton.scale.y;

	float bottom =
		top + prevButton.texSize.y * prevButton.scale.y;

	if (mouseX >= left &&
		mouseX <= right &&
		mouseY >= top &&
		mouseY <= bottom)
	{
		//マウスを乗せると少し拡大
		prevButton.scale.x = TUTORIAL_BUTTON_PREV_PAGE_SCALE_X + 0.1f;
		prevButton.scale.y = TUTORIAL_BUTTON_PREV_PAGE_SCALE_Y + 0.1f;

		if (TRG(0) & PAD_START && timer >= TUTORIAL_SLIDE_FLAME)
		{
			prev_tutorial_number = tutorial_number;
			tutorial_number--;
			if (tutorial_number < 0)
			{
				tutorial_number = 0;
				prev_tutorial_number = -1;
				is_tutorial = false;
			}
			timer = 0;
			music::play(SE_CLICK, false);
		}
	}
	else
	{
		//通常サイズ
		prevButton.scale.x = TUTORIAL_BUTTON_PREV_PAGE_SCALE_X;
		prevButton.scale.y = TUTORIAL_BUTTON_PREV_PAGE_SCALE_Y;
	}
}

void tutorial_next_button_update()
{

	//----------------------------------
	//    マウス座標
	//----------------------------------
	float mouseX = static_cast<float>(getCursorPosX());
	float mouseY = static_cast<float>(getCursorPosY());

	//----------------------------------
	//    ボタンの当たり判定
	//----------------------------------
	float left =
		nextButton.pos.x -
		nextButton.pivot.x * nextButton.scale.x;

	float right =
		left + nextButton.texSize.x * nextButton.scale.x;

	float top =
		nextButton.pos.y -
		nextButton.pivot.y * nextButton.scale.y;

	float bottom =
		top + nextButton.texSize.y * nextButton.scale.y;

	if (mouseX >= left &&
		mouseX <= right &&
		mouseY >= top &&
		mouseY <= bottom)
	{
		//マウスを乗せると少し拡大
		nextButton.scale.x = TUTORIAL_BUTTON_NEXT_PAGE_SCALE_X + 0.1f;
		nextButton.scale.y = TUTORIAL_BUTTON_NEXT_PAGE_SCALE_Y + 0.1f;

		if (TRG(0) & PAD_START && timer >= TUTORIAL_SLIDE_FLAME)
		{
			prev_tutorial_number = tutorial_number;
			tutorial_number++;
			if (tutorial_number >= TUTORIAL_COUNT)
			{
				tutorial_number = 0;
				prev_tutorial_number = -1;
				is_tutorial = false;
			}
			timer = 0;
			music::play(SE_CLICK, false);
		}
	}
	else
	{
		//通常サイズ
		nextButton.scale.x = TUTORIAL_BUTTON_NEXT_PAGE_SCALE_X;
		nextButton.scale.y = TUTORIAL_BUTTON_NEXT_PAGE_SCALE_Y;
	}
}
