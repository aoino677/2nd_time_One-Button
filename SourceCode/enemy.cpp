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
int enemy_max = 4;


void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
	
	for (int i = 0; i <enemy_max ; i++)
	{
		angle[enemy_number] = atan2f(540 - enemy[enemy_number].pos.y, 960 - enemy[enemy_number].pos.x);
		enemy[enemy_number].pos.x += cosf(angle[enemy_number]);
		enemy[enemy_number].pos.y += sinf(angle[enemy_number]);
		enemy_number++;
	}
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
			enemy->scale = { ENEMY_SCALE , ENEMY_SCALE };
			enemy->texSize = { ENEMY_TEX_W , ENEMY_TEX_H };
			enemy->pivot = { ENEMY_PIVOT_X , ENEMY_PIVOT_Y };
			enemy->color = { 1, 1, 1, 1 };
		}
		//enemy_walk();
		//enemy_position();
		
		
		
	
		break;
	

	}

	enemy_timer++;

}

void enemy_render()
{
	sprite_render(
		sprEnemy,
		enemy[enemy_number].pos.x, enemy[enemy_number].pos.y,
		enemy->scale.x, enemy->scale.y,
		0, 0,
		enemy->texSize.x, enemy->texSize.y,
		enemy->pivot.x, enemy->pivot.y,
		ToRadian(0),
		enemy->color.x, enemy->color.y, enemy->color.z, enemy->color.w
	);
	

}