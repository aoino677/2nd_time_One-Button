#pragma once
#include "all.h"

#define EXP_COUNT      (256)      // EXPの個数

#define EXP_SCALE        (0.4f)
#define EXP_TEX_W        (64.0f)
#define EXP_TEX_H        (64.0f)
#define EXP_PIVOT_X      (32.0f)
#define EXP_PIVOT_Y      (32.0f)
// -------------------------------------------------------
// お金ごとの経験値数
// -------------------------------------------------------
#define COUNTER_1Y	(1)
#define COUNTER_10Y	(2)
#define COUNTER_100Y	(4)
#define COUNTER_500Y	(8)


void EXP_init();
void EXP_update();
void EXP_render();
void EXP_deinit();

//生存しているEXPのレベルを全て上げる。
extern void EXP_levelUp_ALL();

// extern宣言
extern Sprite* sprEXP[];
extern OBJ2D EXP[EXP_COUNT];
extern float acquisition_range;