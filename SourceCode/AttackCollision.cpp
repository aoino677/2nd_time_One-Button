/*#include "AttackCollision.h"
#include "Collision2D.h"
#include  "shake.h"

void AttackCollision::player_vs_enemy(OBJ2D& player, OBJ2D& enemy)
{
	VECTOR2 player_begin = player.pos - player.pivot;//playerの左上
	VECTOR2 player_size = { player.scale.x * player.texSize.x, player.scale.y * player.texSize.y };
	VECTOR2 enemy_begin = enemy.pos - enemy.pivot;//enemyの左上
	VECTOR2 enemy_size = { enemy.scale.x * enemy.texSize.x, enemy.scale.y * enemy.texSize.y };
	if (Collision2D::rect_vs_rect(player_begin, player_size, enemy_begin, enemy_size))
	{
		//enemy.hp = 0;
		player.hp--;
	}
}

void AttackCollision::enemy_vs_bullet(OBJ2D& enemy, Bullet& bullet)
{
	VECTOR2 enemy_begin = enemy.pos - VECTOR2{ enemy.scale.x* enemy.pivot.x, enemy.scale.y* enemy.pivot.y };//enemyの左上
	VECTOR2 enemy_size = { enemy.scale.x * enemy.texSize.x, enemy.scale.y * enemy.texSize.y };
	VECTOR2 bullet_center = bullet.pos;
	float bullet_radius = 32;
	if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy.hp > 0)
	{
		enemy.hp -= 1;
		bullet.pos = { -500, -500 };
		
	}
}*/

#include "AttackCollision.h"
#include "Collision2D.h"
#include "shake.h"

void AttackCollision::player_vs_enemy(OBJ2D& player, OBJ2D& enemy)
{
	// プレイヤー側も scale と pivot を考慮して左上を算出
	VECTOR2 player_begin = player.pos - VECTOR2{ player.scale.x * player.pivot.x, player.scale.y * player.pivot.y };
	VECTOR2 player_size = { player.scale.x * player.texSize.x, player.scale.y * player.texSize.y };

	// ★ 敵側も scale と pivot を考慮した正しい計算式に統一する
	VECTOR2 enemy_begin = enemy.pos - VECTOR2{ enemy.scale.x * enemy.pivot.x, enemy.scale.y * enemy.pivot.y };
	VECTOR2 enemy_size = { enemy.scale.x * enemy.texSize.x, enemy.scale.y * enemy.texSize.y };

	if (Collision2D::rect_vs_rect(player_begin, player_size, enemy_begin, enemy_size))
	{
		//enemy.hp = 0;
		player.hp--;
	}
}

void AttackCollision::enemy_vs_bullet(OBJ2D& enemy, Bullet& bullet)
{
	VECTOR2 enemy_begin = enemy.pos - VECTOR2{ enemy.scale.x * enemy.pivot.x, enemy.scale.y * enemy.pivot.y };//enemyの左上
	VECTOR2 enemy_size = { enemy.scale.x * enemy.texSize.x, enemy.scale.y * enemy.texSize.y };
	VECTOR2 bullet_center = bullet.pos;
	float bullet_radius = 32;

	if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy.hp > 0)
	{
		enemy.hp -= 1;
		bullet.pos = { -500, -500 };
	}
}