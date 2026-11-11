#pragma once
#include "all.h"

#define HP_MAX (3)										// 増やすHP量
#define BULLET_SPEED_INCREASED (200.0f)		// 増やすスピード量
#define BULLET_SCALE_INCREASED (0.01f)			// 増やす大きさの量
#define ITEM_LUCKY_UP               (1)				//	アイテムを落とす確率を上げる
#define BULLET_INTERVAL_UP        (1)				// 撃つ間隔が速くなる
#define EFFICACY_MAX (7)								// エフェクトの個数


// 画像
#define POWER_EFFICACY_SCALE     (1.0f)
#define POWER_EFFICACY_TEX_W     (384.0f)
#define POWER_EFFICACY_TEX_H     (768.0f)
#define POWER_EFFICACY_PIVOT_X   (192.0f)
#define POWER_EFFICACY_PIVOT_Y   (384.0f)

extern OBJ2D power_efficacy[EFFICACY_MAX];

extern void HP();
extern void Bullet_speed();
extern void Bullet_quantity();
extern void Bullet_Lucky();
extern void Bullet_scale();
extern void Bullet_rad();
extern void Bullet_Interval();

extern void loser();

