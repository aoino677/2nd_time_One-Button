#include "all.h"
#include "player.h"
#include "enemy.h"
#include "OBJ2D.h"
#include <vector>
#include <cstdlib>


Sprite* sprEnemy;
OBJ2D enemy[ENEMY_NUMBER];
int enemy_state;
int enemy_timer;
int enemy_number = 0;
float enemy_speed=3;
float angle[50];
int enemy_max = ENEMY_NUMBER;


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
		enemy_state++;
		break;

	case 1:
		// パラメーターの設定
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			enemy[i].scale = { ENEMY_SCALE , ENEMY_SCALE };
			enemy[i].texSize = { ENEMY_TEX_W , ENEMY_TEX_H };
			enemy[i].pivot = { ENEMY_PIVOT_X , ENEMY_PIVOT_Y };
			enemy[i].color = { 1, 1, 1, 1 };
			enemy[i].hp = 1;
		}
		//enemy_walk();
		//enemy_position();
		enemy_state++;
		break;
	case 2:
		for (int i = 0; i < enemy_max; i++)
		{
			if (enemy[i].hp <= 0) { continue; }
			angle[i] = atan2f(540 - enemy[i].pos.y, 960 - enemy[i].pos.x);
			enemy[i].pos.x += cosf(angle[i]);
			enemy[i].pos.y += sinf(angle[i]);
		}
	}

	enemy_timer++;

}

void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].hp <= 0) { continue; }
		sprite_render(
			sprEnemy,
			enemy[i].pos.x, enemy[i].pos.y,
			enemy[i].scale.x, enemy[i].scale.y,
			0, 0,
			enemy[i].texSize.x, enemy[i].texSize.y,
			enemy[i].pivot.x, enemy[i].pivot.y,
			ToRadian(0),
			enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w
		);
	}
	

}