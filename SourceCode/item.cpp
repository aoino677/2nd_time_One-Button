#include "item.h"
#include "enemy.h"
#include "player.h"
#include <vector>
#include "Collision2D.h"
#include "EXP.h"

OBJ2D item[ITEM_MAX];
Sprite* spritem[ITEM_MAX];

int item_state = 0;
int random;
int lucky;
int item_couner;
int Blue_counter;

float posX;
float posY;
float LengthX;
float LengthY;
VECTOR2 playerLeftUp;
VECTOR2 playerLength;
float color_w = 0;

void item_init()
{
	item_state = 0;
	item_couner = 0;
	Blue_counter = 0;
	random = 0;
	lucky = ITEM_RANDOM;

	posX = 0;
	posY = 0;
	LengthX = 0;
	LengthY = 0;
}

void item_update()
{
	switch (item_state)
	{
	case 0:
		// 画像追加
		spritem[0] = sprite_load(L"./Data/Images/itemBlue.png");
		spritem[1] = sprite_load(L"./Data/Images/itemGreen.png");
		spritem[2] = sprite_load(L"./Data/Images/itemRed.png");

		item_state++;
		break;
	case 1:
		// パラメーターの設定
		for (int i = 0; i < ITEM_MAX; i++)
		{
			item[i] = {};
			item[i].pos = { enemy[i].pos.x, enemy[i].pos.y };
			item[i].scale		= { ITEM_SCALE,       ITEM_SCALE };
			item[i].texSize	= { ITEM_TEXSIZE_X , ITEM_TEXSIZE_Y };
			item[i].pivot		= { ITEM_PIVOT_X ,   ITEM_PIVOT_Y };
			item[i].color		= { 1, 1, 1, 0 };
		}
		item_state++;
		break;
	case 2:
		for (int enemy_index = 0; enemy_index < enemy_max; enemy_index++)
		{
			// 敵の色が０（敵が死んだら）になったらitemが落ちる
			if (enemy[enemy_index].hp <= 0)
			{
				if (random == rand() % lucky)
				{
					for (int item_index = 0; item_index < ITEM_MAX; item_index++)
					{
						if (item[item_index].color.w != 1.0f)
						{
							item[item_index].pos = enemy[enemy_index].pos;
							item[item_index].color.w = 1.0f;
							item[item_index].moveSpeed = 0;
							break;
						}
					}
				}
			}
		}

		for (int j = 0; j < ITEM_MAX; j++)
		{
			if (item[j].color.w != 1.0f)
			{
				item[j].pos = { -500, -500 };
				continue;
			}
			item[j].moveSpeed += 100.0f / 60.0f;
			item[j].pos.x -= item[j].moveSpeed;
			if (item[j].pos.x < (SCREEN_H - item[j].pos.y) * 0.3584f / 0.9336f)
			{
				item[j].pos.x = (SCREEN_H - item[j].pos.y) * 0.3584f / 0.9336f;
				VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
				posX = item[j].pos.x - (item[j].pivot.x * item[j].scale.x) * acquisition_range;
				posY = item[j].pos.y - (item[j].pivot.y * item[j].scale.y) * acquisition_range;
				LengthX = (item[j].texSize.x * item[j].scale.x) * acquisition_range;
				LengthY = (item[j].texSize.y * item[j].scale.y) * acquisition_range;
				playerLeftUp = player.pos - VECTOR2{ player.pivot.x * player.scale.x, player.pivot.y * player.scale.y };
				playerLength = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
				color_w = 0.3f; // 当たり判定の透明度
				if (Collision2D::rect_vs_rect({ posX, posY }, { LengthX, LengthY }, playerLeftUp, playerLength))
				{
					item_couner++;
					item[j].color.w = 0;
					if (item[0].color.w == 0)
					{
						Blue_counter++;
					}
					color_w = 0.0f;
				}
			}
		}

		if (TRG(0) & PAD_START)
		{
			if (Blue_counter > 0)
			{
				shot_bullet_Ingot();
			}
		}
		break;
	}
}

void item_render()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		sprite_render(
			spritem[i],
			item[i].pos.x, item[i].pos.y,
			item[i].scale.x, item[i].scale.y,
			0, 0,
			item[i].texSize.x, item[i].texSize.y,
			item[i].pivot.x, item[i].pivot.y,
			ToRadian(0),
			1, 1, 1, item[i].color.w
		);
	}
	primitive::rect(
		posX, posY,
		LengthX, LengthY,
		0.0f, 0.0f,
		0.0f,
		0.0f, 0.0f, 1.0f, color_w,
		false
	);

	primitive::rect(
		playerLeftUp.x, playerLeftUp.y,
		playerLength.x, playerLength.y,
		0.0f, 0.0f,
		0.0f,
		0.0f, 1.0f, 0.0f, color_w,
		false
	);

	debug::setString("Blue_counter:%d", Blue_counter);
}

void item_deinit()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		safe_delete(spritem[i]);
	}
	
}
