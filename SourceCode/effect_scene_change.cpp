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
int change_state = 0;        //0: 待機中, 1: 演出中, 2: 完了
int coin_count = 0;          //画面内にいるコイン数
int coin_number = 0;         //コイン番号
int out_count = 0;           //画面外へ落ち切ったコイン数
float coin_width = 0.0f;     //横幅
float coin_height = 0.0f;    //縦幅
float coin_random_x = 0.0f;  //X軸のずれ
float coin_random_y = 0.0f;  //Y軸のずれ

bool scene_switched = false; //切り替え処理済みか
bool retry_mode = false;     //リトライ演出中か
int target_scene = SCENE_NONE;

extern bool result_active;

static void load_coin_sprites()
{
	if (spr1yen == nullptr)   spr1yen = sprite_load(L"Data/Images/1yen.png");
	if (spr10yen == nullptr)  spr10yen = sprite_load(L"Data/Images/10yen.png");
	if (spr100yen == nullptr) spr100yen = sprite_load(L"Data/Images/100yen.png");
	if (spr500yen == nullptr) spr500yen = sprite_load(L"Data/Images/500yen.png");
}

static void setup_coins()
{
	load_coin_sprites();

	coin_count = 0;
	out_count = 0;
	scene_switched = false;

	coin_width = SCREEN_W / GRID_COLS;
	coin_height = SCREEN_H / GRID_ROWS;

	coin_number = 0;
	for (int r = 0; r < GRID_ROWS; r++)
	{
		for (int c = 0; c < GRID_COLS; c++)
		{
			coin_random_x = (float)(rand() % 16 - 8);
			coin_random_y = (float)(rand() % 16 - 8);

			coins[coin_number].x = c * coin_width + (coin_width / 2.0f) + coin_random_x;
			coins[coin_number].target_y = SCREEN_H + 1000.0f + (float)(rand() % 300);
			coins[coin_number].start_y = -(r * coin_height + (float)(rand() % 300 + 50));
			coins[coin_number].y = coins[coin_number].start_y;

			coins[coin_number].vy = 18.0f + (float)(rand() % 8);
			coins[coin_number].rot = ToRadian((float)(rand() % 360));
			coins[coin_number].scale = 1.0f + (float)(rand() % 40) / 100.0f;
			coins[coin_number].type = rand() % 4;
			coin_number++;
		}
	}

	change_state = 1;
}

void scene_change_start(int next_scene_id)
{
	music::play(EFFECT_MONEY_SE, false);
	retry_mode = false;
	target_scene = next_scene_id;
	setup_coins();
}

void scene_change_start_retry()
{

	target_scene = SCENE_NONE;
	retry_mode = true;
	setup_coins();
}

void scene_change_init()
{
	load_coin_sprites();
	change_state = 0;
	coin_count = 0;
	out_count = 0;
	scene_switched = false;
	retry_mode = false;
	target_scene = SCENE_NONE;
	
}

void scene_change_deinit()
{
	if (spr1yen != nullptr) { safe_delete(spr1yen);   spr1yen = nullptr; }
	if (spr10yen != nullptr) { safe_delete(spr10yen);  spr10yen = nullptr; }
	if (spr100yen != nullptr) { safe_delete(spr100yen); spr100yen = nullptr; }
	if (spr500yen != nullptr) { safe_delete(spr500yen); spr500yen = nullptr; }
}

void scene_change_update()
{
	if (change_state != 1)
	{
		return;
	}

	coin_count = 0;
	out_count = 0;

	for (int i = 0; i < TOTAL_COINS; i++)
	{
		//すでに画面外へ落ち切ったコインはスキップ
		if (coins[i].y >= SCREEN_H + 200.0f)
		{
			out_count++;
			continue;
		}

		coins[i].y += coins[i].vy;
		coins[i].rot += 0.05f;

		if (coins[i].y >= 0.0f && coins[i].y <= SCREEN_H)
		{
			coin_count++;
		}

		if (coins[i].y >= SCREEN_H + 200.0f)
		{
			out_count++;
		}
	}

	if (!scene_switched && coin_count >= TOTAL_COINS * 0.85f)
	{
		scene_switched = true;

		if (retry_mode)
		{
			result_active = false;
		}
		else if (target_scene != SCENE_NONE)
		{
			nextScene = target_scene;
		}
	}

	if (out_count >= TOTAL_COINS)
	{
		retry_mode = false;
		target_scene = SCENE_NONE;
		change_state = 2;
	}
}

void scene_change_render()
{
	if (change_state != 1)
	{
		return;
	}

	Sprite* coin_sprites[4] = { spr1yen, spr10yen, spr100yen, spr500yen };

	for (int i = 0; i < TOTAL_COINS; i++)
	{
		//画面外にあるコインは描画しない
		if (coins[i].y < -100.0f || coins[i].y > SCREEN_H + 100.0f)
		{
			continue;
		}

		int type = coins[i].type;
		if (type < 0 || type > 3)
		{
			type = 0;
		}

		Sprite* target_sprite = coin_sprites[type];
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

bool is_scene_changing()
{
	return (change_state == 1);
}

bool is_scene_change_finished()
{
	return (change_state == 2);
}