#include "all.h"
#include "player.h"
#include "enemy.h"
#include "OBJ2D.h"
#include <vector>
#include <cstdlib>


Sprite* sprEnemy;
OBJ2D enemy[50];
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
		1, 1,
		0, 0,
		128, 128,
		64, 64
	);
	

}