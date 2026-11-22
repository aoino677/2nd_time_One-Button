#include "all.h"
#include "result_button.h"
#include "result_score.h"
#include "effect_scene_change.h"
#include "result_screen.h"
#include "enemy.h"
#include "choice.h"
#include "Hp.h"
#include <cmath>

int resulet_state = 0;
int resulet_timer = 0;
bool is_score_set = false;
bool result_scene_bgm_played = false;
float bg_scroll_x = 0.0f;
float bg_width = 1920.0f;
float bg_scroll_speed = 1.5f;

Sprite* sprresult = nullptr;
Sprite* sprResult_logo = nullptr;

void resulet_init()
{
	resulet_state = 0;
	resulet_timer = 0;
	is_score_set = false;
	result_scene_bgm_played = false;
	bg_scroll_x = 0.0f;
	sprresult = nullptr;
	sprResult_logo = nullptr;
	scene_change_init();
	result_button_init();
	result_score_init();
	music::load(EFFECT_MONEY_SE, L"Data/Sounds/SE/money_fall.wav", 1.0f);
}

void resulet_update()
{
	int active_mode = (current_mode != RESULT_MODE_NONE) ? current_mode : confirmed_result_mode;

	switch (resulet_state)
	{
	case 0:
		resulet_state++;
		break;

	case 1:
		GameLib::setBlendMode(Blender::BS_ALPHA);
		if (sprresult == nullptr)
		{
			sprresult = sprite_load(L"Data/Images/result_back.png");
		}
		if (sprResult_logo == nullptr)
		{
			sprResult_logo = sprite_load(L"Data/Images/result.png");
		}

		music::load(RESULT_CLEAR, L"Data/Sounds/BGM/result_clear2.wav");
		music::load(RESULT_OVER, L"Data/Sounds/BGM/result_gameover2.wav");

		resulet_state++;
		break;

	case 2:
		if (!is_score_set)
		{
			result_score_set(score, enemy_kill_count, levelup_counter, player_hp);
			is_score_set = true;
		}

		if (!result_scene_bgm_played)
		{
			if (active_mode == RESULT_MODE_GAMEOVER)
			{
				if (!result_bgm_played)
				{
					music::play(RESULT_OVER, true);
				}
				result_scene_bgm_played = true;
			}
			else if (active_mode == RESULT_MODE_GAMECLEAR)
			{
				if (!result_bgm_played)
				{
					music::play(RESULT_CLEAR, true);
				}
				result_scene_bgm_played = true;
			}
		}

		//îwåiÇç∂Ç÷ÉXÉNÉçÅ[ÉãÇµÇƒÉãÅ[Év
		bg_scroll_x -= bg_scroll_speed;
		if (bg_scroll_x <= -bg_width)
		{
			bg_scroll_x += bg_width;
		}

		result_score_update();
		result_button_update();
		break;
	}

	resulet_timer++;
}

void resulet_render()
{
	//GameLib::clear(1.0f, 0.2f, 0.2f);
	font::textOut(1, "PRESS SPACE KEY", 640, 300);

	//îwåiÉãÅ[Évï`âÊ
	if (sprresult != nullptr)
	{
		sprite_render(sprresult, bg_scroll_x, 0.0f);
		sprite_render(sprresult, bg_scroll_x + bg_width, 0.0f);
	}

	//ÉçÉSÇè„â∫Ç…Ç”ÇÌÇ”ÇÌï`âÊ
	if (sprResult_logo != nullptr)
	{
		float logo_base_y = 20.0f;
		float logo_float_y = sinf(resulet_timer * 0.06f) * 12.0f;

		sprite_render(
			sprResult_logo,
			400.0f, logo_base_y + logo_float_y,
			0.8f, 0.8f
		);
	}

	result_score_render();
	result_button_render();
	scene_change_render();
}

void resulet_deinit()
{
	safe_delete(sprresult);
	sprresult = nullptr;
	safe_delete(sprResult_logo);
	sprResult_logo = nullptr;
	result_button_deinit();
	result_score_deinit();
	music::clear();
	result_scene_bgm_played = false;
}