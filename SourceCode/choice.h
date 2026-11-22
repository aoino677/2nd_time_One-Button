#pragma once
#include "all.h"

#define LEVEL_COUNT_INCREAS (5) // 経験値のマックスを？？？増やす
#define CHOICE_MAX		(3)
// ボタンの画像設定
#define CHOICE_SCALE		(1.0f)
#define CHOICE_TEX_W		(384.0f)
#define CHOICE_TEX_H      (600.0f)
#define CHOICE_PIVOT_X   (192.0f)
#define CHOICE_PIVOT_Y   (300.0f)
#define BUTTON_Y			(600.0f)
// レベルアップ画像設定
#define LEVELUP_POS_X		(950.0f)
#define LEVELUP_POS_Y		(140.0f)
#define LEVELUP_SCALE		(1.0f)
#define LEVELUP_TEX_W		(1300.0f)
#define LEVELUP_TEX_H     (200.0f)
#define LEVELUP_PIVOT_X  (650.0f)
#define LEVELUP_PIVOT_Y  (100.0f)

#define EXP_UP_SE 14

void choice_init();
void choice_update();
void choice_update_pause();
void choice_render();
void choice_deinit();
bool T_or_F(int level_up_count);
void randoms();
void DrawLevelText(int type, float x, float y, float scale);

extern int levelup_counter;

enum
{
	HP_UP,
	WEAPON_SHELL,
	WEAPON_BEAM,
	WEAPON_INGOT,
	WEAPON_BANKROLL,
	ITEM_LUCKY,
	WEAPON_BULLET,
	BULLET_DAMAGE,
	CURSOR_BIG,
	BULLET_INTERVAL,
	HIT_BUCK,
	SCORE
};


