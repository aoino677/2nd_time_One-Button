#pragma once
#include "OBJ2D.h"
//#include "player.h"
//#include "enemy.h"
#include "Bullet.h"

class AttackCollision
{
public:
	static void player_vs_enemy(OBJ2D& player, OBJ2D& enemy);
	static void enemy_vs_bullet(OBJ2D& player, Bullet& enemy);
};