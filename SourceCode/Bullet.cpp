#include "Bullet.h"
#include "enemy.h"
#include "Collision2D.h"
#include "player.h"
#include "Level.h"
#include "../GameLib/game_lib.h"
#include "bullet_se.h"

using namespace GameLib;

const float INGOT_LATE = 0.2f;

Bullet_Y1::Bullet_Y1()
{
	music::play(SE_BULLET_1YEN, false);
}

void Bullet_Y1::update()
{
	old_pos = pos;

	pos += velocity;

	auto* image = new Bullet_Y1_AfterImage{ pos, angle, color_index };

	set_vector_Bullet_Y1_AfterImage(image);
}

void Bullet_Y1::attack(OBJ2D enemies[])
{
	float near_t = FLT_MAX;
	OBJ2D* nearst_enemy = nullptr;
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemies[i].pos - VECTOR2{ enemies[i].scale.x * enemies[i].pivot.x, enemies[i].scale.y * enemies[i].pivot.y };//enemies[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemies[i].scale.x * enemies[i].texSize.x, enemies[i].scale.y * enemies[i].texSize.y };
		VECTOR2 bullet_center = pos;
		float bullet_radius = texSize.x * scale.x * 0.5f;
		if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemies[i].hp > 0)
		{
			if(enemies[i].pos.x < near_t)
			{
				near_t = enemies[i].pos.x;
				nearst_enemy = &enemies[i];
				
			}
		}
		//
		//VECTOR2 hit;
		//float t;
		//if (Collision2D::segment_vs_rect(old_pos, pos, enemy_begin, enemy_size, hit, t))
		//{
		//	if (near_t > t)
		//	{
		//		near_t = t;
		//		nearst_enemy = &enemies[i];
		//	}
		//}
	}

	if (near_t != FLT_MAX)
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