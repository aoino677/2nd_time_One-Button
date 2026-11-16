#include "Bullet.h"
#include "enemy.h"
#include "Collision2D.h"
#include "player.h"
using namespace GameLib;

const float INGOT_LATE = 0.2f;


void Bullet_Y1::update()
{
	pos += velocity;

	auto* image = new Bullet_Y1_AfterImage{ pos, angle, color_index };

	set_vector_Bullet_Y1_AfterImage(image);
}

void Bullet_Y1::attack(OBJ2D enemy[])
{
	float posX = FLT_MAX;
	OBJ2D* nearst_enemy = nullptr;
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemy[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };
		VECTOR2 bullet_center = pos;
		float bullet_radius = texSize.x * scale.x * 0.5f;
		if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy[i].hp > 0)
		{
			if(enemy[i].pos.x < posX)
			{
				posX = enemy[i].pos.x;
				nearst_enemy = &enemy[i];
				
			}
		}
	}

	if (posX != FLT_MAX)
	{
		nearst_enemy->hp -= attack_damage;
		pos = { -500, -500 };//‰æ–ÊŠO‚É
	}
}

void Bullet::render()
{
	sprite_render(
		spr,
		pos.x, pos.y,
		scale.x, scale.y,
		0, 0,
		texSize.x, texSize.y,
		texSize.x * 0.5f, texSize.y * 0.5f,
		angle
	);
}

bool Bullet::is_dead()
{
	return pos.x + texSize.x < 0
		|| pos.x - texSize.x > SCREEN_W
		|| pos.y + texSize.y < 0
		|| pos.y - texSize.y > SCREEN_H;
}
void Bullet_Shell::update()
{
	pos += velocity;
}

void Bullet_Shell::attack(OBJ2D[])
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
				break;
			}
		}
	}
}
