#pragma once
#include "all.h"

#define ITEM_MAX (3)
#define ITEM_SCALE       (1.0f)
#define ITEM_TEXSIZE_X (40.0f)
#define ITEM_TEXSIZE_Y (40.0f)
#define ITEM_PIVOT_X    (20.0f)
#define ITEM_PIVOT_Y    (20.0f)

void item_init();
void item_update();
void item_render();
void item_deinit();