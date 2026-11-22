#include "all.h"
#include "result_button.h"
#include "result_score.h"
#include "effect_scene_change.h"
#include "enemy.h"
#include "Hp.h"
#include <cmath>

int resulet_state = 0;
int resulet_timer = 0;
bool is_score_set = false;
float bg_scroll_x = 0.0f;
float bg_width = 1920.0f;
float bg_scroll_speed = 1.5f;

Sprite* sprresult;
Sprite* sprResult_logo;

void resulet_init()
{
	resulet_state = 0;
	resulet_timer = 0;
	is_score_set = false;
	bg_scroll_x = 0.0f;
	scene_change_init();
	result_button_init();
	result_score_init();
	music::load(RESULET_BGM, L"Data/Sounds/BGM/result_clear3.wav", 0.5f);
	music::load(EFFECT_MONEY_SE, L"Data/Sounds/SE/money_fall.wav", 1.0f);
	music::play(RESULET_BGM, true);
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
		if (!is_score_set)
		{
			result_score_set(score, enemy_kill_count, 99, player_hp);
			is_score_set = true;
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
	GameLib::clear(1.0f, 0.2f, 0.2f);
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
	safe_delete(sprResult_logo);
	result_button_deinit();
	result_score_deinit();
	music::unload(RESULET_BGM);
}