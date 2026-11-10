#include "item.h"

OBJ2D item[ITEM_MAX];
Sprite* spritem[ITEM_MAX];

int item_state = 0;

void item_init()
{
	item_state = 0;
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
			item[i].scale		= { ITEM_SCALE,       ITEM_SCALE };
			item[i].texSize	= { ITEM_TEXSIZE_X , ITEM_TEXSIZE_Y };
			item[i].pivot		= { ITEM_PIVOT_X ,   ITEM_PIVOT_Y };
			item[i].color		= { 1, 1, 1, 1 };
		}
		item_state++;
		break;
	case 2:

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
			1, 1, 1, 1
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
