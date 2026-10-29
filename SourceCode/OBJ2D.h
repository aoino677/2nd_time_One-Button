#pragma once
#include "all.h"

struct OBJ2D
{
    VECTOR2 pos{};                   // 位置
    VECTOR2 scale{ 1.0f, 1.0f };    // スケール（1が自然）
    VECTOR2 texPos{};               // 画像位置
    VECTOR2 texSize{};              // 画像サイズ
    VECTOR2 pivot{};                // 基準点
    VECTOR4 color{ 1,1,1,1 };      // 色（白）

    float moveSpeed = 0;                // キャラクターの移動速度
    float velocity = 0;                     // 球の速度
    float firingSpeed = 0;                // 球の連射速度
    int hp = 0;                       // 体力
    int attack = 0;                 // 攻撃力
    int type;
    int move_timer;

    float timer = 0;                    // タイマー
};