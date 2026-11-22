#include "all.h"
#include "effect_start_game.h"

Sprite* sprWipe = nullptr;

// 0:待機, 1:扉が閉まる移動中, 2:中央バウンド中, 3:閉まり完了
int wipe_in_state = 0;
float wipe_in_move_x1 = 0.0f;
float wipe_in_move_x2 = 0.0f;
float wipe_in_speed_x1 = 115.0f;
float wipe_in_speed_x2 = -115.0f;
int wipe_in_bounce_count = 0;

// wipe_out_state: 0=未開始, 1=開く演出中, 2=開ききり完了, 3=閉じたまま待機中
int wipe_out_state = 0;
float wipe_out_move_x1 = 0.0f;
float wipe_out_move_x2 = 0.0f;
float wipe_out_speed_x1 = -50.0f;
float wipe_out_speed_x2 = 50.0f;

//--------------------------------------
// 初期化・終了
//--------------------------------------
void scene_wipe_init()
{
	if (sprWipe == nullptr)
	{
		sprWipe = sprite_load(L"./Data/Images/effect_scene1.png");
	}

	wipe_in_state = 0;
	wipe_in_bounce_count = 0;
	wipe_out_state = 0;
}

void scene_wipe_deinit()
{
	safe_delete(sprWipe);
	wipe_in_state = 0;
	wipe_in_bounce_count = 0;
	wipe_out_state = 0;
}

//--------------------------------------
// 画面を閉じる演出（イン：扉が閉まって中央で2回バウンド）
//--------------------------------------
void scene_wipe_in_start()
{
	if (sprWipe == nullptr)
	{
		sprWipe = sprite_load(L"./Data/Images/effect_scene1.png");
	}

	wipe_in_state = 1;
	wipe_in_move_x1 = 0.0f;
	wipe_in_move_x2 = 0.0f;
	wipe_in_speed_x1 =100.0f;
	wipe_in_speed_x2 = -100.0f;
	wipe_in_bounce_count = 0;
}

void scene_wipe_in_update()
{
	if (wipe_in_state == 1)
	{
		//左右から中央へ扉を閉める
		wipe_in_move_x1 += wipe_in_speed_x1;
		wipe_in_move_x2 += wipe_in_speed_x2;

		//徐々に減速
		wipe_in_speed_x1 *= 0.91f;
		wipe_in_speed_x2 *= 0.91f;
		if (wipe_in_speed_x1 < 18.0f)  wipe_in_speed_x1 = 18.0f;
		if (wipe_in_speed_x2 > -18.0f) wipe_in_speed_x2 = -18.0f;

		//中央に到達したらバウンドへ
		if (wipe_in_move_x1 >= 960.0f && wipe_in_move_x2 <= -960.0f)
		{
			wipe_in_move_x1 = 960.0f;
			wipe_in_move_x2 = -960.0f;

			//1回目の跳ね返り
			wipe_in_speed_x1 = -20.0f;
			wipe_in_speed_x2 = 20.0f;
			wipe_in_bounce_count = 0;
			wipe_in_state = 2;
		}
	}
	else if (wipe_in_state == 2)
	{
		const float pull_back_x1 = 3.6f;  //扉を中央へ引き戻す力
		const float pull_back_x2 = -3.6f; //扉を中央へ引き戻す力
		const float restitution = 0.42f;  //跳ね返りの減衰率
		const int max_bounces = 2;        //中央でバウンドする回数

		wipe_in_speed_x1 += pull_back_x1;
		wipe_in_speed_x2 += pull_back_x2;

		wipe_in_move_x1 += wipe_in_speed_x1;
		wipe_in_move_x2 += wipe_in_speed_x2;

		//再び中央に戻ってきたらバウンド判定
		if (wipe_in_move_x1 >= 960.0f && wipe_in_move_x2 <= -960.0f)
		{
			wipe_in_move_x1 = 960.0f;
			wipe_in_move_x2 = -960.0f;
			wipe_in_bounce_count++;

			if (wipe_in_bounce_count < max_bounces)
			{
				//2回目の跳ね返り
				wipe_in_speed_x1 = -wipe_in_speed_x1 * restitution;
				wipe_in_speed_x2 = -wipe_in_speed_x2 * restitution;
			}
			else
			{
				//2回バウンドし終えたら完了
				wipe_in_speed_x1 = 0.0f;
				wipe_in_speed_x2 = 0.0f;
				wipe_in_state = 3;
			}
		}
	}
}

void scene_wipe_in_render()
{
	if (sprWipe == nullptr || wipe_in_state == 0) return;

	//左側の扉
	sprite_render(sprWipe, -960.0f + wipe_in_move_x1, 0.0f);

	//右側の扉
	sprite_render(sprWipe, 1920.0f + wipe_in_move_x2, 0.0f);
}

bool is_scene_wipe_in_finished()
{
	return (wipe_in_state == 3);
}


// 画面を開く演出（アウト）
void scene_wipe_out_hold()
{
	if (sprWipe == nullptr)
	{
		sprWipe = sprite_load(L"./Data/Images/effect_scene1.png");
	}

	wipe_out_state = 3; //閉じたまま待機
}

void scene_wipe_out_start()
{
	if (sprWipe == nullptr)
	{
		sprWipe = sprite_load(L"./Data/Images/effect_scene1.png");
	}

	wipe_out_state = 1;
	wipe_out_move_x1 = 0.0f;
	wipe_out_move_x2 = 0.0f;
	wipe_out_speed_x1 = -18.0f;
	wipe_out_speed_x2 = 18.0f;
}

void scene_wipe_out_update()
{
	if (wipe_out_state != 1) return;

	//中央から左右へ開く
	wipe_out_move_x1 += wipe_out_speed_x1;
	wipe_out_move_x2 += wipe_out_speed_x2;

	//徐々に加速
	wipe_out_speed_x1 *= 1.05f;
	wipe_out_speed_x2 *= 1.05f;

	//画面外まで開ききったら完了状態へ
	if (wipe_out_move_x1 <= -960.0f && wipe_out_move_x2 >= 960.0f)
	{
		wipe_out_move_x1 = -960.0f;
		wipe_out_move_x2 = 960.0f;
		wipe_out_state = 2;
	}
}

void scene_wipe_out_render()
{
	if (sprWipe == nullptr) return;

	//カウントダウン終了までは閉じる
	if (wipe_out_state == 3)
	{
		sprite_render(sprWipe, 0.0f, 0.0f);
		sprite_render(sprWipe, 960.0f, 0.0f);
		return;
	}

	//開く演出中だけ描画
	if (wipe_out_state != 1) return;

	//左側の扉
	sprite_render(sprWipe, wipe_out_move_x1, 0.0f);

	//右側の扉
	sprite_render(sprWipe, 960.0f + wipe_out_move_x2, 0.0f);
}

bool is_scene_wipe_out_finished()
{
	return (wipe_out_state == 2);
}