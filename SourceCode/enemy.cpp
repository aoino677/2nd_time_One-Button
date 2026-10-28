#include "all.h"
#include "enemy.h"

Sprite* sprEnemy;
Enemy enemy;
int enemy_state;
int enemy_timer;

void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
}

void enemy_deinit()
{
	safe_delete(sprEnemy);
}

void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		sprEnemy = sprite_load(L"Data/Images/enemy.png");

		break;
		//case 1:

	}
}

void enemy_render()
{
	sprite_render(
		sprEnemy,
		enemy.posX, enemy.posY
	);
}