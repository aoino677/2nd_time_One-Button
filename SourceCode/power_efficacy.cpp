#include "power_efficacy.h"
#include "player.h"
#include "Hp.h"
#include <ctime>
#include <stdlib.h>

// プレイヤーのHPが上がる
void HP()
{
	srand((unsigned int)time(NULL));

	player_hp += rand() % HP_MAX + 1;
}
// 球の発射スピード
void Bullet_speed()
{
	bullet_speed += rand() % 50 + BULLET_SPEED;
}
// 球の発射個数
void Bullet_quantity()
{

}
// 球の大きさ
void Bullet_scale()
{

}
