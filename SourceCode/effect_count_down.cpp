#include "all.h"
#include "effect_count_down.h"

Sprite* sprcount[4] = { nullptr }; // 3, 2, 1, START の画像                  // 0:3, 1:2, 2:1, 3:START
int frame_timer = 0;                  // フレームカウンター
int countdown_state;
int count_phase;
bool is_finished = false;

void countdown_init()
{
	frame_timer = 0;
	countdown_state = 0;
	count_phase = 0;
	is_finished = false;

	for (int i = 0; i < 4; i++)
	{
		safe_delete(sprcount[i]);
	}
}
void countdown_deinit()
{
	for (int i = 0; i < 4; i++)
	{
		safe_delete(sprcount[i]);
	}
}

void countdown_update()
{
	switch (countdown_state)
	{
	case 0:
		sprcount[0] = sprite_load(L"./Data/Images/count_3.png");
		sprcount[1] = sprite_load(L"./Data/Images/count_2.png");
		sprcount[2] = sprite_load(L"./Data/Images/count_1.png");
		sprcount[3] = sprite_load(L"./Data/Images/count_start.png");

		countdown_state++;
		break;

	case 1:
		frame_timer++;

		if (frame_timer >= 30)
		{
			frame_timer = 0;
			count_phase++;

			if (count_phase >= 4)
			{
				is_finished = true;
			}
		}
		break;
	}
}

void countdown_render()
{
	if (count_phase >= 4)
		return;

	if (sprcount[count_phase] == nullptr)
		return;

	sprite_render(
		sprcount[count_phase],
		960.0f, 640.0f,
		0.5f, 0.5f,
		0.0f, 0.0f,
		512.0f, 512.0f,
		256.0f, 256.0f
	);
}
bool countdown_finished()
{
	return is_finished;
}

