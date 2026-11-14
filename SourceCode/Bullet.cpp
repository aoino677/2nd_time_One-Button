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

Bullet_INGOT::~Bullet_INGOT()
{
	for (auto* ingot : ingots)
	{
		delete ingot;
	}
	ingots.clear();
}

void Bullet_INGOT::update()
{
	for (auto* ingot : ingots)
	{
		ingot->velocity += accel_velocity;
		ingot->pos += ingot->velocity;
	}

	for (auto itr = ingots.begin(); itr != ingots.end();)
	{
		if ((*itr)->pos.y - texSize.y > SCREEN_H)
		{
			delete *itr;
			itr = ingots.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	debug::setString("%d", ingots.size());

	if (timer > INGOT_LATE)
	{
		timer -= INGOT_LATE;
		auto* ingot = new Ingot();
		ingot->pos.x = pos.x;
		ingot->pos.y = 0;
		ingots.push_back(ingot);
	}

	timer += 1 / system::FRAME_RATE;
}

void Bullet_INGOT::attack(OBJ2D enemy[])
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		for (auto* ingot : ingots)
		{
			VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemy[e]‚Ì¶ã
			VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };
			VECTOR2 bullet_center = ingot->pos;
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
}

void Bullet_INGOT::render()
{
	for (auto* ingot : ingots)
	{
		sprite_render(
			spr,
			ingot->pos.x, ingot->pos.y,
			scale.x, scale.y,
			0, 0,
			texSize.x, texSize.y,
			texSize.x * 0.5f, texSize.y * 0.5f,
			angle
		);
	}
}

void Bullet_SHELL::update()
{
	pos += velocity;
}

void Bullet_SHELL::attack(OBJ2D[])
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
