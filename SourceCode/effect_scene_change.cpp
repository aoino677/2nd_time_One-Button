#include "all.h"
#include "effect_scene_change.h"
#include "result_screen.h"
#include "scene_game.h"
#include <stdlib.h>

Sprite* spr1yen = nullptr;
Sprite* spr10yen = nullptr;
Sprite* spr100yen = nullptr;
Sprite* spr500yen = nullptr;

Coin coins[TOTAL_COINS];
int change_state = 0;
int coin_count = 0;       //コインカウント
int coin_number = 0;      //コイン番号
int out_count = 0;        //落ちたコインカウント
float coin_width = 0.0f;    //横幅
float coin_height = 0.0f;   //縦幅
float coin_random_x = 0.0f; //X軸のずれ
float coin_random_y = 0.0f; //Y軸のずれ
bool scene_switched = false; //切り替え処理済みか
bool retry_mode = false;     //リトライ演出中か

extern bool result_active;

//リトライ時にコイン演出を開始する
void scene_change_start_retry()
{
	scene_change_init();
	retry_mode = true;
}

void scene_change_init()
{
	change_state = 0;
	coin_count = 0;
	out_count = 0;
	scene_switched = false;
	retry_mode = false;

	coin_width = SCREEN_W / GRID_COLS;
	coin_height = SCREEN_H / GRID_ROWS;

	coin_number = 0;
	for (int r = 0; r < GRID_ROWS; r++)
	{
		for (int c = 0; c < GRID_COLS; c++)
		{
			//ランダムなズレを少し持たせてバラつかせる
			coin_random_x = (float)(rand() % 16 - 8);
			coin_random_y = (float)(rand() % 16 - 8);

			coins[coin_number].x = c * coin_width + (coin_width / 2.0f) + coin_random_x;

			//目標地点ははるか下（画面外）
			coins[coin_number].target_y = SCREEN_H + 1000.0f + (float)(rand() % 300);

			//上空からのスタート位置
			coins[coin_number].start_y = -(r * coin_height + (float)(rand() % 300 + 50));
			coins[coin_number].y = coins[coin_number].start_y;

			//落下速度・回転
			coins[coin_number].vy = 18.0f + (float)(rand() % 8);
			coins[coin_number].rot = ToRadian((float)(rand() % 360));

			//1.0倍〜1.4倍程度
			coins[coin_number].scale = 1.0f + (float)(rand() % 40) / 100.0f;

			coins[coin_number].type = rand() % 4;
			coin_number++;
		}
	}
}

void scene_change_deinit()
{
	if (spr1yen != nullptr) { safe_delete(spr1yen); spr1yen = nullptr; }
	if (spr10yen != nullptr) { safe_delete(spr10yen); spr10yen = nullptr; }
	if (spr100yen != nullptr) { safe_delete(spr100yen); spr100yen = nullptr; }
	if (spr500yen != nullptr) { safe_delete(spr500yen); spr500yen = nullptr; }
}

void scene_change_update()
{
	switch (change_state)
	{
	case 0:
		if (spr1yen == nullptr) spr1yen = sprite_load(L"Data/Images/1yen.png");
		if (spr10yen == nullptr) spr10yen = sprite_load(L"Data/Images/10yen.png");
		if (spr100yen == nullptr) spr100yen = sprite_load(L"Data/Images/100yen.png");
		if (spr500yen == nullptr) spr500yen = sprite_load(L"Data/Images/500yen.png");

		change_state++;
		break;

	case 1:
		//毎フレームリセット
		coin_count = 0;

		//それぞれのコインを落下させる
		for (int i = 0; i < TOTAL_COINS; i++)
		{
			coins[i].y += coins[i].vy;
			coins[i].rot += 0.05f;

			if (coins[i].y >= 0.0f && coins[i].y <= SCREEN_H)
			{
				coin_count++;
			}
		}

		//コインが85%以上になったら切り替え処理
		if (!scene_switched && coin_count >= TOTAL_COINS * 0.85f)
		{
			scene_switched = true;

			if (retry_mode)
			{
				result_active = false;
				game_init();
			}
			else
			{
				nextScene = SCENE_MENU;
			}
		}

		//全てのコインが画面外へ完全に落ち切ったかチェック
		out_count = 0;
		for (int i = 0; i < TOTAL_COINS; i++)
		{
			if (coins[i].y >= SCREEN_H + 200.0f)
			{
				out_count++;
			}
		}

		if (out_count >= TOTAL_COINS)
		{
			retry_mode = false;
			change_state = 2;
		}
		break;

	case 2:
		break;
	}
}

void scene_change_render()
{
	if (change_state < 1)
	{
		return;
	}

	for (int i = 0; i < TOTAL_COINS; i++)
	{
		if (coins[i].y < -200.0f || coins[i].y > SCREEN_H + 600.0f)
		{
			continue;
		}

		Sprite* target_sprite = spr1yen;
		if (coins[i].type == 1) target_sprite = spr10yen;
		else if (coins[i].type == 2) target_sprite = spr100yen;
		else if (coins[i].type == 3) target_sprite = spr500yen;

		if (target_sprite != nullptr)
		{
			sprite_render(
				target_sprite,
				coins[i].x, coins[i].y,
				coins[i].scale, coins[i].scale,
				0.0f, 0.0f,
				64.0f, 64.0f,
				32.0f, 32.0f,
				coins[i].rot,
				1.0f, 1.0f, 1.0f, 1.0f,
				false
			);
		}
	}
}

//エフェクトが完全に終わったか（state2になったか）を返す
bool is_scene_change_finished()
{
	return (change_state == 2);
}