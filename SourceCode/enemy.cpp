#include "all.h"
#include "player.h"
#include "enemy.h"
#include <vector>


Player;
Sprite* sprEnemy;
Enemy enemy;
int enemy_state;
int enemy_timer;
float enemy_speed = 3;


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
	{
		enemy_walk();
		break;
	}
	
		
	
	
	
	
	}
}


void enemy_walk()
{
    //Šp“x‚ğŒvZ‚µ‚ÄˆÚ“®
	//Œã‚Åplayer.pos‚É•ÏX‚·‚é
	float angle = atan2f(540 - enemy.pos.y, 960 - enemy.pos.x);
	enemy.pos.x += cosf(angle) * enemy_speed;
	enemy.pos.y += sinf(angle) * enemy_speed;
}


void enemy_render()
{
	sprite_render(
		sprEnemy,
		enemy.pos.x, enemy.pos.y,
		1,1,
	    0,0,
		128,128,
		64,64
	);
}