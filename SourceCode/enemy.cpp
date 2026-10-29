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
	
		enemy_walk();
		enemy_position();
		
		
			
		

		
	
		break;
	

	}

	enemy_timer++;

}





void enemy_position()
{
	if (enemy_timer > 240)
	{
		for (int i = 0; i < 4; i++)
		{
			enemy[enemy_number].pos.x = rand();
			enemy[enemy_number].pos.y = rand();
			enemy_number++;
		}

	}
	else
		enemy_timer = 0;

}


void enemy_walk()
{
    //Šp“x‚ðŒvŽZ‚µ‚ÄˆÚ“®
	//Œã‚Åplayer.pos‚É•ÏX‚·‚é
	float angle = atan2f(540 - enemy[enemy_number].pos.y, 960 - enemy[enemy_number].pos.x);
	enemy[enemy_number].pos.x += cosf(angle) * enemy_speed;
	enemy[enemy_number].pos.y += sinf(angle) * enemy_speed;
}


void enemy_render()
{
	for (int i=0; i < 4; i++)
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

}