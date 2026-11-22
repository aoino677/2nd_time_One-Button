#include "all.h"
#include "effect_count_down.h"

Sprite* sprcount[4] = { nullptr }; // 3, 2, 1, START の画像
int frame_timer = 0;                 //フレームカウンター
int start_delay_timer = 0;           //開始前の待機フレームカウンター
int countdown_state;
int count_phase;
bool is_finished = false;

int START_DELAY_FRAMES = 30; //シーン切り替わり後の待機フレーム数
int COUNT_DISP_FRAMES = 35;  //数字・STARTを表示するフレーム数
int COUNT_GAP_FRAMES = 10;   //切り替わり前後の余白フレーム数
int COUNT_TOTAL_FRAMES = COUNT_DISP_FRAMES + COUNT_GAP_FRAMES;

void countdown_init()
{
	frame_timer = 0;
	start_delay_timer = START_DELAY_FRAMES;
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

		start_delay_timer = START_DELAY_FRAMES;
		countdown_state = 1;
		break;

	case 1:
		//待機フレーム中はカウントダウンを進めない
		if (start_delay_timer > 0)
		{
			start_delay_timer--;
			return;
		}

		frame_timer = 0;
		count_phase = 0;
		countdown_state = 2;
		break;

	case 2:
		if (is_finished)
			return;

		frame_timer++;

		if (frame_timer >= COUNT_TOTAL_FRAMES)
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
	if (countdown_state != 2)
		return;

	if (count_phase >= 4)
		return;

	//余白中は描画しない
	if (frame_timer >= COUNT_DISP_FRAMES)
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