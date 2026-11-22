#include "all.h"
#include "result_button.h"
#include "result_score.h"
#include "effect_scene_change.h"
#include "enemy.h"
#include "Hp.h"

int resulet_state = 0;
int resulet_timer = 0;
static bool is_score_set = false; 

Sprite* sprresult;
Sprite* sprResult_logo;

void resulet_init()
{
	resulet_state = 0;
	resulet_timer = 0;
	is_score_set = false;
	scene_change_init();
	result_button_init();
	result_score_init();
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
		// 例：リザルト初期化時やゲーム終了時に渡す
		if (!is_score_set)
		{
			result_score_set(12345, enemy_kill_count, 99, player_hp);
			is_score_set = true;
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

	sprite_render(
		sprresult,
		0, 0
	);

	sprite_render(
		sprResult_logo,
		400, 0,
		0.8, 0.8
	);

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
}