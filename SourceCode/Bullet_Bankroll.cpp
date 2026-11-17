#include "Bullet_Bankroll.h"
#include "Collision2D.h"
#include "enemy.h"


Bullet_Bankroll::~Bullet_Bankroll()
{
}

void Bullet_Bankroll::update()
{
	angle += ToRadian(200) / system::FRAME_RATE;

	if (angle > ToRadian(180))
	{
		angle -= ToRadian(360);
		attacked_enemies.clear();
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
		VECTOR2 enemy_begin = enemy[i].pos;
		VECTOR2 enemy_pivot = VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemies[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };

		if (Collision2D::OBB_vs_OBB(pos, s, piv, angle, enemy_begin, enemy_size, enemy_pivot, 0))
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

void Bullet_Bankroll::render()
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

bool Bullet_Bankroll::is_dead()
{
	//return angle > ToRadian(90);
	return false;
}
