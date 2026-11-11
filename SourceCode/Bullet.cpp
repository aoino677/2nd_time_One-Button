#include "Bullet.h"
#include "enemy.h"
using namespace GameLib;


void Bullet_Y1::update()
{
	pos += velocity;
}

void Bullet_Y1::attack(OBJ2D& enemy)
{
	if (end_pos.y - pos.y < enemy.texSize.y * enemy.scale.y * 0.5f)
	{
		enemy.hp -= attack_damage;
		pos = { -500, -500 };//‰æ–ÊŠO‚É
	}
}

void Bullet_INGOT::update()
{
	velocity.y += 3.0f / system::FRAME_RATE;
	pos += velocity;
}

void Bullet_INGOT::attack(OBJ2D& enemy)
{
	bool check = false;
	for (auto* e : attacked_enemies)
	{
		if (e == &enemy)
		{
			check = true;
			break;
		}
	}

	if (!check)
	{
		enemy.hp -= attack_damage;
		attacked_enemies.push_back(&enemy);
	}
}
