#include "Bullet_Bankroll.h"
#include "Collision2D.h"
#include "enemy.h"


Bullet_Bankroll::~Bullet_Bankroll()
{
}

void Bullet_Bankroll::update()
{
	angle += ToRadian(200) / system::FRAME_RATE;

}

void Bullet_Bankroll::attack(OBJ2D enemies[])
{
	VECTOR2 p, vec, svec;
	vec.x = pivot.x * scale.x;
	vec.y = pivot.y * scale.y;
	p.x = pos.x - (vec.x * cosf(angle) + vec.y * -sinf(angle));
	p.y = pos.y - (vec.x * sinf(angle) + vec.y * cosf(angle));
	svec.x = texSize.x * scale.x;
	svec.y = texSize.y * scale.y;

	VECTOR2 A, B, C, D;
	A = p;
	B = { p.x + (svec.x * cosf(angle)), p.y + (svec.x * sinf(angle)) };
	C = { p.x + svec.x * cosf(angle) + svec.y * -sinf(angle), p.y + svec.x * sinf(angle) + svec.y * cosf(angle) };
	D = { p.x + svec.y * -sinf(angle), p.y + svec.y * cosf(angle) };

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{

		VECTOR2 enemy_begin = enemy[i].pos - VECTOR2{ enemy[i].scale.x * enemy[i].pivot.x, enemy[i].scale.y * enemy[i].pivot.y };//enemies[e]‚Ì¶ã
		VECTOR2 enemy_size = { enemy[i].scale.x * enemy[i].texSize.x, enemy[i].scale.y * enemy[i].texSize.y };

		if (Collision2D::triangle_vs_rect(A, B, C, enemy_begin, enemy_size)
			|| Collision2D::triangle_vs_rect(A, D, C, enemy_begin, enemy_size))
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
	return angle > ToRadian(90);
}
