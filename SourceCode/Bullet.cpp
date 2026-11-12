#include "Bullet.h"
#include "enemy.h"
#include "Collision2D.h"
using namespace GameLib;


void Bullet_Y1::update()
{
	pos += velocity;
}

void Bullet_Y1::attack(OBJ2D enemy[])
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemy[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };
		VECTOR2 bullet_center = pos;
		float bullet_radius = texSize.x * scale.x * 0.5f;
		if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy[i].hp > 0)
		{
			{
				enemy[i].hp -= attack_damage;
				pos = { -500, -500 };//‰æ–ÊŠO‚É
			}
		}
	}
}

void Bullet_INGOT::update()
{
	velocity += accel_velocity;
	pos += velocity;
}

void Bullet_INGOT::attack(OBJ2D enemy[])
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemy[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };
		VECTOR2 bullet_center = pos;
		float bullet_radius = texSize.x * scale.x * 0.5f;
		if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy[i].hp > 0)
		{
			{
				bool check = false;
				for (auto* e : attacked_enemies)
				{
					if (e == &enemy[i])
					{
						check = true;
						break;
					}
				}

				if (!check)
				{
					enemy[i].hp -= attack_damage;
					attacked_enemies.push_back(&enemy[i]);
				}
			}
		}
	}
}
