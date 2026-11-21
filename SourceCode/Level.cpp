#include "Level.h"
#include "player.h"
#include "cursor_main.h"
#include "power_efficacy.h"
#include <cmath>

int level_up_weapon[WEAPON_MAX];

// ダメージ・攻撃範囲の計算
float CalculateGrowth(float B, int L, float r, float k, float P = 1.0f,float T = 1.0f)
{
    float level = static_cast<float>(L - 1);

    return B * powf(r, level) * (1.0f + k * level * level) * P * T;
}

// クールタイムの計算
float CalculateCooltime(float B, int L, float r, float k, float Bmin, float P = 1.0f,float T = 1.0f)
{
    float level = static_cast<float>(L - 1);
    // 現在レベルの成長量
    float numerator = powf(r, level) * (1.0f + k * level * level) - 1.0f;

    // 最大レベルの成長量
    float max_level = static_cast<float>(WEAPON_LEVEL_MAX - 1);
    float denominator = powf(r, max_level) * (1.0f + k * max_level * max_level) - 1.0f;

    // クールタイム計算
    float result = Bmin + (B - Bmin) * (1.0f - numerator / denominator);
    return result / (P * T);
}

void level_init()
{
    for (int i = 0; i < WEAPON_MAX; i++) { level_up_weapon[i] = 0; }
    level_up_weapon[Y1] = level_1;
}

void level_update()
{
    // 一円ライフル
    int level = level_up_weapon[Y1];

    if (level < level_1) level = level_1;
    if (level > WEAPON_LEVEL_MAX) level = WEAPON_LEVEL_MAX;

    level_up_weapon[Y1] = level;

    // ダメージ
    float damage = CalculateGrowth(Y1_SETTING.damage_base, level, Y1_SETTING.damage_rate, Y1_SETTING.damage_growth, Y1_SETTING.power_rate, Y1_SETTING.time_rate);
    Y1_damage = Y1_DAMAGE * damage / 100.0f;

    // クールタイム
    float cooltime = CalculateCooltime(Y1_SETTING.cooltime_base, level, Y1_SETTING.cooltime_rate, Y1_SETTING.cooltime_growth, Y1_SETTING.cooltime_min, 1.0f / bullet_interval_rate);
    timer_down_bullet = static_cast<int>(TIMER * cooltime / Y1_SETTING.cooltime_base + 0.5f);

    if (timer_down_bullet < 1) timer_down_bullet = 1;

    // 攻撃範囲
    float range = CalculateGrowth(Y1_SETTING.range_base, level, Y1_SETTING.range_rate, Y1_SETTING.range_growth);
    bullet_scale = Y1_SETTING.scale_base * range / 100.0f;


    //==================================================
    // 貝殻ミサイル
    //==================================================

    int shell_level = level_up_weapon[SHELL];

    if (shell_level > 0)
    {
        if (shell_level > WEAPON_LEVEL_MAX)
            shell_level = WEAPON_LEVEL_MAX;

        level_up_weapon[SHELL] = shell_level;

        // ダメージ
        float damage = CalculateGrowth(
            SHELL_SETTING.damage_base,
            shell_level,
            SHELL_SETTING.damage_rate,
            SHELL_SETTING.damage_growth,
            SHELL_SETTING.power_rate,
            SHELL_SETTING.time_rate
        );

        shell_damage = SHELL_DAMAGE * damage / 100.0f;

        // クールタイム
        float cooltime = CalculateCooltime(
            SHELL_SETTING.cooltime_base,
            shell_level,
            SHELL_SETTING.cooltime_rate,
            SHELL_SETTING.cooltime_growth,
            SHELL_SETTING.cooltime_min,
            1.0f / red_cooltime_rate,
            1.0f / bullet_interval_rate
        );

        timer_down_shell =
            static_cast<int>(TIMER_SHELL * cooltime / SHELL_SETTING.cooltime_base + 0.5f);

        if (timer_down_shell < 1) timer_down_shell = 1;

        // 攻撃範囲
        float range = CalculateGrowth(
            SHELL_SETTING.range_base,
            shell_level,
            SHELL_SETTING.range_rate,
            SHELL_SETTING.range_growth
        );

        shell_scale = SHELL_SETTING.scale_base * range / 100.0f;
    }


    //==================================================
    // インゴット
    //==================================================

    int ingot_level = level_up_weapon[INGOT];

    if (ingot_level > 0)
    {
        if (ingot_level > WEAPON_LEVEL_MAX)
            ingot_level = WEAPON_LEVEL_MAX;

        level_up_weapon[INGOT] = ingot_level;

        // ダメージ
        float damage = CalculateGrowth(
            INGOT_SETTING.damage_base,
            ingot_level,
            INGOT_SETTING.damage_rate,
            INGOT_SETTING.damage_growth,
            INGOT_SETTING.power_rate,
            INGOT_SETTING.time_rate
        );

        ingot_damage = INGOT_DAMAGE * damage / 100.0f;

        // クールタイム
        float cooltime = CalculateCooltime(
            INGOT_SETTING.cooltime_base,
            ingot_level,
            INGOT_SETTING.cooltime_rate,
            INGOT_SETTING.cooltime_growth,
            INGOT_SETTING.cooltime_min,
            1.0f / red_cooltime_rate,
            1.0f / bullet_interval_rate
        );

        timet_down_ingot =
            static_cast<int>(TIMER_INGOT * cooltime / INGOT_SETTING.cooltime_base + 0.5f);

        if (timet_down_ingot < 1) timet_down_ingot = 1;

        // 攻撃範囲
        float range = CalculateGrowth(
            INGOT_SETTING.range_base,
            ingot_level,
            INGOT_SETTING.range_rate,
            INGOT_SETTING.range_growth
        );

        ingot_scale = INGOT_SETTING.scale_base * range / 100.0f;
    }


    //==================================================
    // ワンコインフォール（BEAM）
    //==================================================

    int beam_level = level_up_weapon[BEAM];

    if (beam_level > 0)
    {
        if (beam_level > WEAPON_LEVEL_MAX)
            beam_level = WEAPON_LEVEL_MAX;

        level_up_weapon[BEAM] = beam_level;

        // ダメージ
        float damage = CalculateGrowth(
            BEAM_SETTING.damage_base,
            beam_level,
            BEAM_SETTING.damage_rate,
            BEAM_SETTING.damage_growth,
            BEAM_SETTING.power_rate,
            BEAM_SETTING.time_rate
        );

        beem_damage = BEEM_DAMAGE * damage / 100.0f;

        // クールタイム
        float cooltime = CalculateCooltime(
            BEAM_SETTING.cooltime_base,
            beam_level,
            BEAM_SETTING.cooltime_rate,
            BEAM_SETTING.cooltime_growth,
            BEAM_SETTING.cooltime_min,
            1.0f / red_cooltime_rate,
            1.0f / bullet_interval_rate
        );

        timer_down_beam =
            static_cast<int>(TIMER_BEAM * cooltime / BEAM_SETTING.cooltime_base + 0.5f);

        if (timer_down_beam < 1) timer_down_beam = 1;

        // 攻撃範囲
        float range = CalculateGrowth(
            BEAM_SETTING.range_base,
            beam_level,
            BEAM_SETTING.range_rate,
            BEAM_SETTING.range_growth
        );

        beem_scale = BEAM_SETTING.scale_base * range / 100.0f;
    }


    //==================================================
    // 札束ピンタ（BANKROLL）
    //==================================================

    int bankroll_level = level_up_weapon[BANKROLL];

    if (bankroll_level > 0)
    {
        if (bankroll_level > WEAPON_LEVEL_MAX)
            bankroll_level = WEAPON_LEVEL_MAX;

        level_up_weapon[BANKROLL] = bankroll_level;

        // ダメージ
        float damage = CalculateGrowth(
            BANKROLL_SETTING.damage_base,
            bankroll_level,
            BANKROLL_SETTING.damage_rate,
            BANKROLL_SETTING.damage_growth,
            BANKROLL_SETTING.power_rate,
            BANKROLL_SETTING.time_rate
        );

        bankrool_damage = 20.0f * damage / 100.0f;

        // クールタイム
        float cooltime = CalculateCooltime(
            BANKROLL_SETTING.cooltime_base,
            bankroll_level,
            BANKROLL_SETTING.cooltime_rate,
            BANKROLL_SETTING.cooltime_growth,
            BANKROLL_SETTING.cooltime_min,
            1.0f / red_cooltime_rate,
            1.0f / bullet_interval_rate
        );

        timer_down_bankroll =
            static_cast<int>(TIMER_BANKROLL * cooltime / BANKROLL_SETTING.cooltime_base + 0.5f);

        if (timer_down_bankroll < 1) timer_down_bankroll = 1;

        // 攻撃範囲
        float range = CalculateGrowth(
            BANKROLL_SETTING.range_base,
            bankroll_level,
            BANKROLL_SETTING.range_rate,
            BANKROLL_SETTING.range_growth
        );

        bankrool_scale = BANKROLL_SETTING.scale_base * range / 100.0f;
    }
}