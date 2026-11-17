#include "Bullet_Shell.h"
#include"enemy.h"
#include "Collision2D.h"

void Bullet_Shell::update()
{
	pos += velocity;
}

void Bullet_Shell::attack(OBJ2D[])
{
	VECTOR2 s;
	s.x = scale.x + texSize.x;
	s.y = scale.y + texSize.y;
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemies[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };
		if (Collision2D::OBB_vs_OBB(pos, s, {0,0}, 0, enemy_begin, enemy_size, {0,0}, 0))
		{
			{
				enemy[i].hp -= attack_damage;
				pos = { -500, -500 };//‰æ–ÊŠO‚É
				break;
			}
		}
	}
}
