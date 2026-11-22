#pragma once
#include "all.h"

enum WEAPON_LEVEL
{
    level_1 = 1,
    level_2,
    level_3,
    level_4,
    level_5,
    level_6,
    level_7,
    level_8
};

enum WEAPON_TYPE
{
    Y1,
    SHELL,
    INGOT,
    BEAM,
    BANKROLL,
    WEAPON_MAX
};

struct WEAPON_LEVEL_SETTING
{
    // ダメージ
    float damage_base;       // 基礎値
    float damage_rate;       // 基本倍率
    float damage_growth;     // 成長係数

    // クールタイム
    float cooltime_base;     // 基礎値
    float cooltime_rate;     // 基本倍率
    float cooltime_growth;   // 成長係数
    float cooltime_min;      // 最低値

    // 攻撃範囲
    float range_base;        // 基礎値
    float range_rate;        // 基本倍率
    float range_growth;      // 成長係数

    // 強化倍率
    float power_rate;        // 強化倍率
    float time_rate;         // 一時強化倍率

    // 弾の基本スケール
    float scale_base;
};

//==================================================
// プランナー調整用設定
//==================================================

// 一円ライフル
const WEAPON_LEVEL_SETTING Y1_SETTING =
{
    // ダメージ
    100.0f, 1.1f, 0.02f,
    // クールタイム
    200.0f, 1.07f, 0.03f, 50.0f,
    // 攻撃範囲
    100.0f, 1.02f, 0.01f,
    // 強化倍率
    1.0f, 1.0f,
    // 弾の基本スケール
    1.0f
};

// 貝殻ミサイル
const WEAPON_LEVEL_SETTING SHELL_SETTING =
{
    // ダメージ
    500.0f, 1.05f, 0.02f,
    // クールタイム
    1600.0f, 1.02f, 0.03f, 700.0f,
    // 攻撃範囲
    100.0f, 1.02f, 0.01f,
    // 強化倍率
    1.0f, 1.0f,
    // 弾の基本スケール
    2.5f
};

// インゴット
const WEAPON_LEVEL_SETTING INGOT_SETTING =
{
    // ダメージ
    80.0f, 1.02f, 0.05f,
    // クールタイム
    1100.0f, 1.02f, 0.03f, 500.0f,
    // 攻撃範囲
    50.0f, 1.02f, 0.01f,
    // 強化倍率
    1.0f, 1.0f,
    // 弾の基本スケール
    1.5f
};

// 札束ビンタ
const WEAPON_LEVEL_SETTING BANKROLL_SETTING =
{
    // ダメージ
    100.0f, 1.05f, 0.1f,
    // クールタイム
    1500.0f, 1.05f, 0.03f, 800.0f,
    // 攻撃範囲
    80.0f, 1.01f, 0.01f,
    // 強化倍率
    1.0f, 1.0f,
    // 弾の基本スケール
    1.0f
};

// ワンコインフォール
const WEAPON_LEVEL_SETTING BEAM_SETTING =
{
    // ダメージ
    80.0f, 1.03f, 0.02f,
    // クールタイム
    1200.0f, 1.05f, 0.03f, 600.0f,
    // 攻撃範囲
    100.0f, 1.02f, 0.01f,
    // 強化倍率
    1.0f, 1.0f,
    // 弾の基本スケール
    1.5f
};

#define WEAPON_LEVEL_MAX level_8

extern int level_up_weapon[WEAPON_MAX];
extern float Y1_attack_range;

void level_init();
void level_update();