#include "item.h"
#include "enemy.h"
#include "player.h"
#include <vector>
#include "Collision2D.h"
#include <ctime>
#include <stdlib.h>

OBJ2D item[ITEM_MAX];
Sprite* spritem[ITEM_MAX];

int item_state = 0;
int random;
int lucky;
int item_couner;

void item_init()
{
	item_state = 0;
	item_couner = 0;
	random = 0;
	lucky = ITEM_RANDOM;
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
				float posX = item[j].pos.x - item[j].pivot.x * item[j].scale.x;
				float posY = item[j].pos.y - item[j].pivot.y * item[j].scale.y;
				float LengthX = item[j].texSize.x * item[j].scale.x;
				float LengthY = item[j].texSize.y * item[j].scale.y;
				VECTOR2 playerLeftUp = player.pos - VECTOR2{ player.pivot.x * player.scale.x, player.pivot.y * player.scale.y };
				VECTOR2 playerLength = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
				if (Collision2D::rect_vs_rect({ posX, posY }, { LengthX, LengthY }, playerLeftUp, playerLength))
				{
					/*if (spritem[0])
					{
						bullet_ingot_counter++;
					}*/
					item_couner++;
					item[j].color.w = 0;
				}
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
}

void item_deinit()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		safe_delete(spritem[i]);
	}
	
}
