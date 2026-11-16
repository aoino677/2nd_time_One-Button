#include "Bullet.h"
#include "enemy.h"
#include "Collision2D.h"
#include "player.h"
using namespace GameLib;

const float INGOT_LATE = 0.2f;

bool segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2 length, VECTOR2& hit_pos, float& min_t);

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
		//if (segment_vs_rect(old_pos, pos, enemy_begin, enemy_size, hit, t))
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
void Bullet_Shell::update()
{
	pos += velocity;
}

void Bullet_Shell::attack(OBJ2D[])
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemies[e]‚Ì¶ã
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

inline float crossZ(VECTOR2 a, VECTOR2 b)
{
	return a.y * b.x - a.x * b.y;
}

bool segment_ve_segment(const VECTOR2& a, const VECTOR2& b, const VECTOR2& c, const VECTOR2& d, VECTOR2& hit_pos, float& t)
{
	VECTOR2 AB = b - a;
	VECTOR2 CA = a - c;
	VECTOR2 CD = d - c;
	t = crossZ(-CD, CA) / crossZ(CD, AB);
	float u = crossZ(-AB, -CA) / crossZ(AB, CD);
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		hit_pos = a + AB * t;
		return true;
	}
	return false;
}

bool segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2 length, VECTOR2& hit_pos, float& min_t)
{
	VECTOR2 LU{ pos };
	VECTOR2 RU{ pos.x + length.x, pos.y };
	VECTOR2 LD{ pos.x , pos.y + length.y };
	VECTOR2 RD{ pos + length };

	if (a.x > pos.x && a.x < pos.x + length.x
		&& a.y > pos.y && a.y < pos.y + length.y)
	{
		hit_pos = a;
		return true;
	}

	VECTOR2 hit;
	float t = 0;
	min_t = FLT_MAX;
	bool boo = false;

	if (segment_ve_segment(a, b, LU, RU, hit, t))
	{
		if (min_t > t)
		{
			min_t = t;
			hit_pos = hit;
		}
		boo = true;
	}
	if (segment_ve_segment(a, b, RD, RU, hit, t))
	{
		if (min_t > t)
		{
			min_t = t;
			hit_pos = hit;
		}
		boo = true;
	}
	if (segment_ve_segment(a, b, RD, LD, hit, t))
	{
		if (min_t > t)
		{
			min_t = t;
			hit_pos = hit;
		}
		boo = true;
	}
	if (segment_ve_segment(a, b, LU, LD, hit, t))
	{
		if (min_t > t)
		{
			min_t = t;
			hit_pos = hit;
		}
		boo = true;
	}
	return boo;
}
