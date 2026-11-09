#include "choice.h"
#include "level_up.h"
#include "power_efficacy.h"
#include "player.h"
#include <ctime>
#include <stdlib.h>

OBJ2D choiceButton[CHOICE_MAX];
Sprite* sprchoicebar;
OBJ2D power_efficacy[EFFICACY_MAX]; // 効能の個数
Sprite* sprpower_efficacy[EFFICACY_MAX];

int choice_state = 0;
int choice_timer = 0;
int level_up_count = 0;
int choice_num = 0;

float choice_effect_y = 0.0f;
float choice_effect_alpha = 0.0f;

void choice_init()
{
	choice_state = 0;
	choice_timer = 0;
	level_up_count = 0;
	bullet_lay_count = 1;
	choice_num = 0;
	choice_effect_y = 0.0f;
	choice_effect_alpha = 0.0f;

	// choiceButtonの読み込み
	sprchoicebar = sprite_load(L"./Data/Images/choicebar.png");
	// 文の読み込み
	sprpower_efficacy[0] = sprite_load(L"./Data/images/HP_efficacy.png");
	sprpower_efficacy[1] = sprite_load(L"./Data/images/Bullet_speed_efficacy.png");
	sprpower_efficacy[2] = sprite_load(L"./Data/images/Bullet_quantity_efficacy.png");
	sprpower_efficacy[3] = sprite_load(L"./Data/images/Bullet_diffusion_efficacy.png");
	sprpower_efficacy[4] = sprite_load(L"./Data/images/Bullet_scale_efficacy.png");
	sprpower_efficacy[5] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[6] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[7] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[8] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[9] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[10] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");
	//sprpower_efficacy[11] = sprite_load(L"./Data/images/Bullet_rad_efficacy.png");

	// パラメーターの設定
	for (int i = 0; i < CHOICE_MAX; i++)
	{
		choiceButton[i] = {};
		//choiceButton[i].pos = { 0, 0 };
		choiceButton[i].scale = { static_cast<float>(CHOICE_SCALE)    , static_cast<float>(CHOICE_SCALE) };
		choiceButton[i].texSize = { static_cast<float>(CHOICE_TEX_W)    , static_cast<float>(CHOICE_TEX_H) };
		choiceButton[i].pivot = { static_cast<float>(CHOICE_PIVOT_X)  ,static_cast<float>(CHOICE_PIVOT_Y) };
		choiceButton[i].color = { 1, 1, 1, 1 };
	}
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		power_efficacy[i] = {};
		power_efficacy[i].scale = { POWER_EFFICACY_SCALE ,POWER_EFFICACY_SCALE };
		power_efficacy[i].texSize = { POWER_EFFICACY_TEX_W , POWER_EFFICACY_TEX_H };
		power_efficacy[i].pivot = { POWER_EFFICACY_PIVOT_X , POWER_EFFICACY_PIVOT_Y };
		power_efficacy[i].color = { 1, 1, 1, 1 };
	}
	
	choiceButton[0].pos = { 450, 500 };
	choiceButton[1].pos = { 950, 500 };
	choiceButton[2].pos = { 1450, 500 };

	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		power_efficacy[i].pos = choiceButton[0].pos;
		power_efficacy[i].pos = choiceButton[1].pos;
		power_efficacy[i].pos = choiceButton[2].pos;
	}
}

void choice_update_pause()
{
	if (!choice_pause)
	{
		if (exp_count >= level_max || TRG(0) & PAD_TRG2)
		{
			level_up_count++;
			for (int i = 1; i <= level_up_count; i++)
			{
				for (; T_or_F(level_up_count);)
				{
					level_max += 5;
					exp_count = 0;
					choice_pause = true;
					choice_effect_y = static_cast<float>(SCREEN_H);
					choice_effect_alpha = 0.0f;
					choice_state = 0;
					level_up_count--;
				}
				if (i == level_up_count)
				{
					level_up_count = 0;
				}
			}
		}
	}
}

void choice_update()
{
	switch (choice_state)
	{
	case 0:
		//----------------------------------
		// 演出開始
		//----------------------------------
		choice_effect_y = static_cast<float>(SCREEN_H);
		choice_effect_alpha = 0.0f;

		choice_state++;
		break;
	case 1:
		// 背景を徐々に暗くする
		choice_effect_alpha += 0.04f;

		if (choice_effect_alpha >= 0.8f)
		{
			choice_effect_alpha = 0.8f;
		}

		// ボタンを下から上へ移動
		choice_effect_y -= 40.0f;

		if (choice_effect_y <= 0.0f)
		{
			choice_effect_y = 0.0f;
		}

		// 演出終了
		if (choice_effect_y <= 0.0f &&
			choice_effect_alpha >= 0.8f)
		{
			choice_state++;
		}
		break;
	case 2:
		for (int i = 0; i < CHOICE_MAX; i++)
		{
			float mouseX = static_cast<float>(getCursorPosX());
			float mouseY = static_cast<float>(getCursorPosY());

			float buttonY = choiceButton[i].pos.y + choice_effect_y;
			float left = choiceButton[i].pos.x - choiceButton[i].pivot.x * choiceButton[i].scale.x;
			float right = choiceButton[i].pos.x + (choiceButton[i].texSize.x - choiceButton[i].pivot.x) * choiceButton[i].scale.x;
			float top = buttonY - choiceButton[i].pivot.y * choiceButton[i].scale.y;
			float bottom = buttonY + (choiceButton[i].texSize.y - choiceButton[i].pivot.y) * choiceButton[i].scale.y;
			//----------------------------------
			// ポーズボタンの判定
			//----------------------------------
			if (mouseX >= left &&
				mouseX <= right &&
				mouseY >= top &&
				mouseY <= bottom)
			{
				choiceButton[i].scale.x = CHOICE_SCALE + 0.1f;
				choiceButton[i].scale.y = CHOICE_SCALE + 0.1f;

				if (TRG(0) & PAD_START)
				{
					srand((unsigned int)time(NULL));
					switch (i)
					{
					case 0:
						choice_pause = false;
						choice_state = 0;
						choice_num = 0;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (rand() % 6)
						{
						case 0:
							HP(choice_num);
							break;
						case 1:
							Bullet_scale(choice_num);
							break;
						case 2:
							Bullet_speed(choice_num);
							break;
						case 3:
							Bullet_rad(choice_num);
							break;
						case 4:
							Bullet_quantity(choice_num);
							break;
						case 5:
							Bullet_diffusion(choice_num);
							break;
						}
						break;

					case 1:
						choice_pause = false;
						choice_state = 0;
						choice_num = 1;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (rand() % 6)
						{
						case 0:
							HP(choice_num);
							break;
						case 1:
							Bullet_scale(choice_num);
							break;
						case 2:
							Bullet_speed(choice_num);
							break;
						case 3:
							Bullet_rad(choice_num);
							break;
						case 4:
							Bullet_quantity(choice_num);
							break;
						case 5:
							Bullet_diffusion(choice_num);
							break;
						}
						break;

					case 2:
						choice_pause = false;
						choice_state = 0;
						choice_num = 2;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (rand() % 6)
						{
						case 0:
							HP(choice_num);
							break;
						case 1:
							Bullet_scale(choice_num);
							break;
						case 2:
							Bullet_speed(choice_num);
							break;
						case 3:
							Bullet_rad(choice_num);
							break;
						case 4:
							Bullet_quantity(choice_num);
							break;
						case 5:
							Bullet_diffusion(choice_num);
							break;
						}
						break;
					}
				}
				else
				{
					choiceButton[i].scale.x = CHOICE_SCALE;
					choiceButton[i].scale.y = CHOICE_SCALE;
				}
				return;
			}
		}
		break;
	}
}

void choice_render()
{
	if (choice_pause == true)
	{
	//----------------------------------
   // 暗い背景
   //----------------------------------
		primitive::rect(
			0.0f, 0.0f,
			static_cast<float>(SCREEN_W),
			static_cast<float>(SCREEN_H),
			0.0f, 0.0f,
			0.0f,
			0.0f, 0.0f, 0.0f,
			choice_effect_alpha
		);
		for (int i = 0; i < CHOICE_MAX; i++)
		{
			sprite_render(
				sprchoicebar,
				choiceButton[i].pos.x, choiceButton[i].pos.y + choice_effect_y,
				choiceButton[i].scale.x, choiceButton[i].scale.y,
				0, 0,
				choiceButton[i].texSize.x, choiceButton[i].texSize.y,
				choiceButton[i].pivot.x, choiceButton[i].pivot.y,
				ToRadian(0),
				1, 1, 1, 1
			);
		}
		for (int i = 0; i < EFFICACY_MAX; i++)
		{
			sprite_render(
				sprpower_efficacy[i],
				power_efficacy[i].pos.x, power_efficacy[i].pos.y,
				power_efficacy[i].scale.x, power_efficacy[i].scale.y,
				0, 0,
				power_efficacy[i].texSize.x, power_efficacy[i].texSize.y,
				power_efficacy[i].pivot.x, power_efficacy[i].pivot.y,
				ToRadian(0),
				1, 1, 1, 1
			);
		}
	}
}

void choice_deinit()
{
	safe_delete(sprchoicebar);
}

bool T_or_F(int level_up_count)
{
	if (level_up_count == 0) { return false; }
	else { return true; }
}
