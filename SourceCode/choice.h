#pragma once
#include "all.h"

#define CHOICE_MAX           (3)
#define CHOICE_SCALE         (1.0f)
#define CHOICE_TEX_W        (384.0f)
#define CHOICE_TEX_H         (768.0f)
#define CHOICE_PIVOT_X      (192.0f)
#define CHOICE_PIVOT_Y      (384.0f)

void choice_init();
void choice_update();
void choice_render();
void choice_deinit();
