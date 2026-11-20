#include "effect_game_start.h"
#include "effect_count_down.h"

Sprite* sprWall;

int effect_game_start_state;

float effect_game_start_speed_x1 = 0.0f;
float effect_game_start_speed_x2 = 0.0f;

float effect_game_start_slow_x1 = -50.0f;
float effect_game_start_slow_x2 = 50.0f;

void effect_game_start_init()
{
	safe_delete(sprWall);
	effect_game_start_state = 0;

	effect_game_start_speed_x1 = 0.0f;
	effect_game_start_speed_x2 = 0.0f;

	effect_game_start_slow_x1 = -50.0f;
	effect_game_start_slow_x2 = 50.0f;


}

void effect_game_start_deinit()
{
	safe_delete(sprWall);
}

void effect_game_start_update()
{
	switch (effect_game_start_state)
	{
	case 0:
		sprWall = sprite_load(L"./Data/Images/effect_scene1.png");
		effect_game_start_state++;
		break;

	case 1:
		countdown_update();
		if (countdown_finished())
			effect_game_start_state++;
	case 2:
		effect_game_start_speed_x1 += effect_game_start_slow_x1;
		effect_game_start_speed_x2 += effect_game_start_slow_x2;

		effect_game_start_slow_x1 *= 1.01f;
		effect_game_start_slow_x2 *= 1.01f;

		if (effect_game_start_speed_x1 < -960.0f)
			effect_game_start_speed_x1 = -960.0f;

		if (effect_game_start_speed_x2 > 960.0f)
			effect_game_start_speed_x2 = 960.0f;

		break;
	}
}

void effect_game_start_render()
{
	//カウントダウン中は数字を描画
	if (!countdown_finished())
	{
		countdown_render();
	}
	
	if (sprWall == nullptr)
		return;
	sprite_render(
		sprWall,
		effect_game_start_speed_x1,
		0.0f
	);

	sprite_render(
		sprWall,
		960.0f + effect_game_start_speed_x2,
		0.0f
	);

}
