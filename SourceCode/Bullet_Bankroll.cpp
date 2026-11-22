#include "Bullet_Bankroll.h"
#include "Collision2D.h"
#include "enemy.h"
#include "../GameLib/game_lib.h"
#include "bullet_se.h"

Bullet_Bankroll::~Bullet_Bankroll()
{
}

void Bullet_Bankroll::update()
{
	if (angle <= roll_max)
	{

		angle += roll_speed / system::FRAME_RATE;

		if (angle > ToRadian(180))
		{
			angle -= ToRadian(360);
			attacked_enemies.clear();
		}
	}

	for (auto& image : after_images)
	{
		image.color.w -= 0.1f;
	}

	for (auto itr = after_images.begin(); itr != after_images.end();)
	{
		if (itr->color.w < 0)
		{
			itr = after_images.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	if (angle <= roll_max)
	{
		after_images.push_back(after_image());
		after_images.back().pos = pos;
		after_images.back().pivot = pivot;
		after_images.back().angle =	angle;
		after_images.back().color = { 1, 0.2f, 0.6f, 1 };
	}
}

void Bullet_Bankroll::attack(OBJ2D enemies[])
{
	VECTOR2 piv;
	piv.x = pivot.x * scale.x;
	piv.y = pivot.y * scale.y;
	VECTOR2 s;
	s.x = texSize.x * scale.x;
	s.y = texSize.y * scale.y;

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		if (enemies[i].hp <= 0) continue;
		VECTOR2 enemy_begin = enemies[i].pos;
		VECTOR2 enemy_pivot = VECTOR2{ enemies[i].scale.x * enemies[i].pivot.x, enemies[i].scale.y * enemies[i].pivot.y };//enemies[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemies[i].scale.x * enemies[i].texSize.x, enemies[i].scale.y * enemies[i].texSize.y };

		if (Collision2D::OBB_vs_OBB(pos, s, piv, angle, enemy_begin, enemy_size, enemy_pivot, 0))
		{
			bool check = false;
			for (auto* e : attacked_enemies)
			{
				if (e == &enemies[i])
				{
					check = true;
					break;
				}
			}

			if (!check)
			{
				enemies[i].hp -= attack_damage;
				attacked_enemies.push_back(&enemies[i]);
				if (is_hit_least_once == false)
				{
					music::play(SE_BULLET_BANKROLL, false);
					is_hit_least_once = true;
				}
			}
		}
	}
}

void Bullet_Bankroll::render()
{
	for (auto image : after_images)
	{
		sprite_render(
			spr,
			image.pos.x, image.pos.y,
			scale.x, scale.y,
			0, 0,
			texSize.x, texSize.y,
			image.pivot.x, image.pivot.y,
			image.angle,
			image.color.x, image.color.y, image.color.z, image.color.w
		);
	}

	if (angle <= ToRadian(100))
	{
		sprite_render(
			spr,
			pos.x, pos.y,
			scale.x, scale.y,
			0, 0,
			texSize.x, texSize.y,
			pivot.x, pivot.y,
			angle
		);
	}
}

bool Bullet_Bankroll::is_dead()
{
	return after_images.size() <= 0;
	false;
}
