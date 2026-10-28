#pragma once
#include "all.h"

struct OBJ2D
{
    VECTOR2 pos{};          // 位置
    VECTOR2 scale{ 1.0f, 1.0f }; // スケール（1が自然）
    VECTOR2 texPos{};       // 画像位置
    VECTOR2 texSize{};      // 画像サイズ
    VECTOR2 pivot{};        // 基準点
    VECTOR4 color{ 1,1,1,1 }; // 色（白）
};