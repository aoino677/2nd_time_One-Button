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
		// ‰æ‘œ’Ç‰Á
		spritem[0] = sprite_load(L"./Data/Images/cursor.png");
	}
}

void item_render()
{
}

void item_deinit()
{
}
