#include "power_efficacy.h"
#include "player.h"
#include "Hp.h"
#include "Bullet.h"
#include "cursor_main.h"
#include "item.h"
#include "EXP.h"
#include "enemy.h"
#include <ctime>
#include <stdlib.h>

int damage_up;
int efficacy_timer;
void efficacy_init()
{
	damage_up = 0;
	efficacy_timer = 0;
}
// プレイヤーのHPが上がる
void HP()
{
	player_hp += rand() % HP_MAX + 1;
}
// 球の発射スピード
void Bullet_speed()
{
	bullet_speed += rand() % 50 + BULLET_SPEED_INCREASED;
}
// 球の発射個数
void Bullet_quantity()
{
	bullet_lay_count++;
}
// 球の大きさ
void Bullet_scale()
{
	bullet_scale += BULLET_SCALE_INCREASED;
}
// 弾の狙った方向に飛ぶ精度
void Bullet_rad(float rad)
{
	precision_rad -= ToRadian(rad);
}

// アイテムが落ちやすくなる
void Bullet_Lucky()
{
	lucky -= ITEM_LUCKY_UP;
}
// 弾の撃つ間隔が短くなる
void Bullet_Interval()
{
	timer_down -= BULLET_INTERVAL_UP;
}
// プレイヤーの攻撃力を増やす
void Bullet_Damage()
{
	damage_up += BULLET_DAMAGE_UP;
}
// 落とし物の取得範囲を広げる
void Lost_Item_Acquisition_Range()
{
	acquisition_range += LOST_ITEM_ACQUISITION_RANGE_UP;
}
// HPの最大量を上げる
void HP_Max()
{
	max_hp += 3;
}
//-------BULE----------------
void Fever_Time_Start_Blue()
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		enemy[i].moveSpeed = 0;
	}
}
// 終わり
void Fever_Time_End_Blue()
{
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		enemy[i].moveSpeed = 1 + enemy_speed_up;
	}
}
//-------BULE----------------
// 
//-------RED----------------
// 一定時間あほみたいに強くする
void Fever_Time_Start_Red()
{
	lucky += 50;
	timer_down -= 10;
}
// 終わり
void Fever_Time_End_Red()
{
	lucky -= 50;
	timer_down += 10;
}
//-------RED----------------


// はずれ
void loser()
{
	bullet_lay_count += 0;
}
