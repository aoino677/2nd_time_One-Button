#include "AttackCollision.h"
#include "Collision2D.h"

void AttackCollision::player_vs_enemy(OBJ2D& player, OBJ2D& enemy)
{
	VECTOR2 player_begin = player.pos - player.pivot;//playerÇÃç∂è„
	VECTOR2 player_size = player.scale + player.texSize;
	VECTOR2 enemy_begin = enemy.pos - enemy.pivot;//enemyÇÃç∂è„
	VECTOR2 enemy_size = enemy.scale + enemy.texSize;
	if (Collision2D::rect_vs_rect(player_begin, player_size, enemy_begin, enemy_size))
	{
		enemy.scale = { 0, 0 };
	}
}