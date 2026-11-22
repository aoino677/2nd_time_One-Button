#pragma once
#include "all.h"

#define ITEM_MAX (256)
#define ITEM_RANDOM (5) // 50回に1回
#define ITEM_TIMER (120) // アイテム取得までの時間

#define ITEM_SCALE       (1.0f)
#define ITEM_TEXSIZE_X (40.0f)
#define ITEM_TEXSIZE_Y (40.0f)
#define ITEM_PIVOT_X    (20.0f)
#define ITEM_PIVOT_Y    (20.0f)

#define REDITEM_EFFECT_SCALE       (3.0f)
#define REDITEM_EFFECT_TEXSIZE_X (64.0f)
#define REDITEM_EFFECT_TEXSIZE_Y (47.0f)
#define REDITEM_EFFECT_PIVOT_X    (32.0f)
#define REDITEM_EFFECT_PIVOT_Y    (23.5f)

#define ITEM_BLUE_SE1 8
#define ITEM_BLUE_SE2 9
#define ITEM_PINK_SE   10
#define ITEM_RED_SE		11
#define ITME_GREEN_SE 12

void item_init();
void item_update();
void item_render();
void item_deinit();

// 敵が死亡したときに呼び出す
void item_drop(int enemy_index);

extern bool fever_blue;
extern int lucky;