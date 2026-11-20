#pragma once
#include "all.h"

#define ITEM_MAX (256)
#define ITEM_RANDOM (10) // 50回に1回
#define ITEM_TIMER (120) // アイテム取得までの時間

#define ITEM_SCALE       (1.0f)
#define ITEM_TEXSIZE_X (40.0f)
#define ITEM_TEXSIZE_Y (40.0f)
#define ITEM_PIVOT_X    (20.0f)
#define ITEM_PIVOT_Y    (20.0f)

void item_init();
void item_update();
void item_render();
void item_deinit();

// 敵が死亡したときに呼び出す
void item_drop(int enemy_index);

extern bool fever_blue;
extern int lucky;