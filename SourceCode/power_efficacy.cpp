#include "power_efficacy.h"
#include "player.h"
#include "Hp.h"
#include "Bullet.h"
#include "cursor_main.h"
#include "item.h"
#include "EXP.h"
#include "enemy.h"
#include "Level.h"
#include <ctime>
#include <stdlib.h>

float damage_up;
int efficacy_timer;
float red_cooltime_rate;
float bullet_interval_rate;
int hp_level;
int lucky_level;
int bullet_damage_level;
int cursor_level;
int bullet_interval_level;
int hit_buck_level;
void efficacy_init()
{
	damage_up = 0.0f;
	efficacy_timer = 0;
	red_cooltime_rate = 1.0f;
	bullet_interval_rate = 1.0f;

	hp_level = 1;
	lucky_level = 1;
	bullet_damage_level = 1;
	cursor_level = 1;
	bullet_interval_level = 1;
	hit_buck_level = 1;
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
    if (lucky_level <= 15)
    {
        lucky_level++;
    }

    lucky -= ITEM_LUCKY_UP;

    if (lucky < 5)
    {
        lucky = 5; // 上限はここを触る
    }
}
// 弾の撃つ間隔が短くなる
void Bullet_Interval()
{
	if (bullet_interval_level < level_7)
	{
		bullet_interval_level++;
		bullet_interval_rate -= 0.05f; // プランナーはここを触る
	}
	if (bullet_interval_rate < 0.5f)
	{
		bullet_interval_rate = 0.5f;
	}
}
// プレイヤーの攻撃力を増やす
void Bullet_Damage()
{
	if (bullet_damage_level < level_7)
	{
		bullet_damage_level++;
		damage_up += BULLET_DAMAGE_UP;
	}
}
// 落とし物の取得範囲を広げる
void Lost_Item_Acquisition_Range()
{
	acquisition_range += LOST_ITEM_ACQUISITION_RANGE_UP;
}
// HPの最大量を上げる
void HP_Max()
{
	if (hp_level < level_5)
	{
		hp_level++;
		max_hp += HP_MAX_UP;
	}
}

// ヒットバックする距離を増やす
void Hit_Bag()
{
	if (hit_buck_level < level_5)
	{
		hit_buck_level++;
		hit_buck += 1.0f; // ここを触ってください
	}
}
// スコアを上げる
void Score_Up()
{
	// ??? += 1000;
}
//-------BULE----------------
void Fever_Time_Start_Blue()
{
	fever_blue = true;
}
// 終わり
void Fever_Time_End_Blue()
{
	fever_blue = false;
}
//-------BULE----------------
//
//-------RED----------------
// 一定時間あほみたいに強くする
void Fever_Time_Start_Red()
{
	lucky += 30;
	red_cooltime_rate = 0.05f;
}
// 終わり
void Fever_Time_End_Red()
{
	lucky -= 30;
	red_cooltime_rate = 1.0f;
}
//-------RED----------------


// はずれ
void loser()
{
	bullet_lay_count += 0;
}
