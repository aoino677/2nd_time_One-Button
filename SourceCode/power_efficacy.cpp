#include "power_efficacy.h"
#include "player.h"
#include "Hp.h"
#include "Bullet.h"
#include "cursor_main.h"
#include "item.h"
#include "EXP.h"
#include <ctime>
#include <stdlib.h>

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
void Bullet_rad()
{
	precision_rad -= ToRadian(0.5f);
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
	Y1_damage += BULLET_DAMAGE_UP;
}
// 落とし物の取得範囲を広げる
void Lost_Item_Acquisition_Range()
{
	acquisition_range += LOST_ITEM_ACQUISITION_RANGE_UP;
}

// はずれ
void loser()
{
	player_hp += 0;
	bullet_speed += 0;
	bullet_lay_count += 0;
	precision_rad += 0;
	bullet_lay_gap += 0;
}