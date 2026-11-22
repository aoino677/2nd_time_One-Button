#include "Bullet_Ingot.h"
#include "enemy.h"
#include "Collision2D.h"
#include "bullet_se.h"

Bullet_Ingot::Bullet_Ingot(Type type)
{
	switch (type)
	{
	case Type::Ingot:
		music::play(SE_BULLET_INGOT, false);
		break;
	case Type::Issen:
		music::play(SE_BULLET_ISSEN, false);
		break;
	}
}

Bullet_Ingot::~Bullet_Ingot()
{
#ifdef CAN_RAPIDSHOT_BULLET_INGOT
	for (auto* ingot : ingots)
	{
		delete ingot;
	}
	ingots.clear();
#endif
}

void Bullet_Ingot::update()
{
	velocity += accel_velocity;
	pos += velocity;

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

	if (pos.x + texSize.x > 0
	 && pos.x - texSize.x < SCREEN_W
	 && pos.y + texSize.y > 0
	 && pos.y - texSize.y < SCREEN_H)
	{
		after_images.push_back(after_image());
		after_images.back().pos = pos;
		after_images.back().color = { 0, 1, 0, 1 };
	}


#ifdef CAN_RAPIDSHOT_BULLET_INGOT
	for (auto* ingot : ingots)
	{
		ingot->velocity += accel_velocity;
		ingot->pos += ingot->velocity;
	}
	for (auto itr = ingots.begin(); itr != ingots.end();)
	{
		if ((*itr)->pos.y - texSize.y > SCREEN_H)
		{
			delete* itr;
			itr = ingots.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	if (timer > INGOT_LATE)
	{
		timer -= INGOT_LATE;
		auto* ingot = new Ingot();
		ingot->pos.x = pos.x;
		ingot->pos.y = 0;
		ingots.push_back(ingot);
	}

	timer += 1 / system::FRAME_RATE;
#endif
}

void Bullet_Ingot::attack(OBJ2D eneies[])
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		//for (auto* ingot : ingots)
		{
			VECTOR2 enemy_begin = eneies[i].pos - VECTOR2{ eneies[i].scale.x * eneies[i].pivot.x, eneies[i].scale.y * eneies[i].pivot.y };//eneies[e]‚Ì¶ã
			VECTOR2 enemy_size = { eneies[i].scale.x * eneies[i].texSize.x, eneies[i].scale.y * eneies[i].texSize.y };
			//VECTOR2 bullet_center = ingot->pos;
			VECTOR2 bullet_center = pos;
			float bullet_radius = texSize.x * scale.x * 0.5f;
			if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && eneies[i].hp > 0)
			{
				{
					bool check = false;
					for (auto* e : attacked_enemies)
					{
						if (e == &eneies[i])
						{
							check = true;
							break;
						}
					}

					if (!check)
					{
						eneies[i].hp -= attack_damage;
						attacked_enemies.push_back(&eneies[i]);
					}
				}
			}
		}
	}
}

void Bullet_Ingot::render()
{
	for (auto image : after_images)
	{
		sprite_render(
			spr,
			image.pos.x, image.pos.y,
			// ingot->pos.x, ingot->pos.y,
			scale.x, scale.y,
			0, 0,
			texSize.x, texSize.y,
			texSize.x * 0.5f, texSize.y * 0.5f,
			angle,
			image.color.x, image.color.y, image.color.z, image.color.w
		);
	}

	//for (auto* ingot : ingots)
	{
		sprite_render(
			spr,
			pos.x, pos.y,
			// ingot->pos.x, ingot->pos.y,
			scale.x, scale.y,
			0, 0,
			texSize.x, texSize.y,
			texSize.x * 0.5f, texSize.y * 0.5f,
			angle
		);
	}
}

bool Bullet_Ingot::is_dead()
{
	return after_images.size() <= 0;
	//return pos.x + texSize.x < 0
	//	|| pos.x - texSize.x > SCREEN_W
	//	|| pos.y + texSize.y < 0
	//	|| pos.y - texSize.y > SCREEN_H;
}
