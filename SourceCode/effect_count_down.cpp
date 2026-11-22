#include "all.h"
#include "effect_count_down.h"

Sprite* sprcount[4] = { nullptr }; //3, 2, 1, START の画像
int frame_timer = 0;                 //フレームカウンター
int start_delay_timer = 0;           //開始前の待機フレームカウンター
int countdown_state;
int count_phase;
bool is_finished = false;

int START_DELAY_FRAMES = 45; //シーン切り替わり後の待機フレーム数
int COUNT_DISP_FRAMES = 15;  //数字・STARTを表示するフレーム数
int COUNT_GAP_FRAMES = 20;   //切り替わり前後の余白フレーム数
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

	if (is_finished || count_phase >= 4)
		return;

	//余白中は描画しない
	if (frame_timer >= COUNT_DISP_FRAMES)
		return;

	if (sprcount[count_phase] == nullptr)
		return;

	float tex_w = 600.0f;
	float tex_h = 200.0f;
	float pivot_x = tex_w * 0.5f;
	float pivot_y = tex_h * 0.5f;

	float pos_x = 960.0f;
	float pos_y = 540.0f;
	float scale_x = 0.5f;
	float scale_y = 0.5f;
	float angle = 0.0f;

	if (count_phase == 0)
	{
		//3
		pos_x = 1160.0f;
		pos_y = 540.0f;
		scale_x = 0.6f;
		scale_y = 0.6f;
		angle = 0.20f;
	}
	else if (count_phase == 1)
	{
		//2
		pos_x = 760.0f;
		pos_y = 540.0f;
		scale_x = 0.85f;
		scale_y = 0.85f;
		angle = -0.20f;
	}
	else if (count_phase == 2)
	{
		//1
		pos_x = 960.0f;
		pos_y = 540.0f;
		scale_x = 1.00f;
		scale_y = 1.00f;
		angle = 0.0f;
	}
	else if (count_phase == 3)
	{
		//GO!
		pos_x = 960.0f;
		pos_y = 540.0f;
		scale_x = 1.2f;
		scale_y = 1.2f;
		angle = 0.0f;
	}

	sprite_render(
		sprcount[count_phase],
		pos_x, pos_y,
		scale_x, scale_y,
		0.0f, 0.0f,
		tex_w, tex_h,
		pivot_x, pivot_y,
		angle
	);
}

bool countdown_finished()
{
	return is_finished;
}