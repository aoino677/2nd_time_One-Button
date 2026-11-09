#include "all.h"
#include "obj2d.h"
#include "cursor_main.h"
#include "player.h"

//==================================================
// 変数
//==================================================

Sprite* sprCursor_main = nullptr;
OBJ2D cursor_main;

// 状態
int cursor_main_state = 0;
int cursor_main_timer = 0;

// マウス位置
float cursor_main_posX = 0.0f;
float cursor_main_posY = 0.0f;

//==================================================
// 初期化
//==================================================

void cursor_main_init()
{
    safe_delete(sprCursor_main);
    cursor_main_timer = 0;
    cursor_main_state = 0;

    cursor_main_posX = 0.0f;
    cursor_main_posY = 0.0f;

    cursor_main = {};
}


//==================================================
// 終了処理
//==================================================

void cursor_main_deinit()
{
    safe_delete(sprCursor_main);
}


//==================================================
// 更新処理
//==================================================

void cursor_main_update()
{
    switch (cursor_main_state)
    {
    case 0:
        // 画像読み込み
        sprCursor_main = sprite_load(L"./Data/Images/cursor.png");

        cursor_main_state++;

        // わざとbreakしない

    case 1:
        cursor_main = {};
        cursor_main.pos     = { 0.0f, 0.0f };
        cursor_main.scale   = { CURSOL_MAIN_SCALE,CURSOL_MAIN_SCALE };
        cursor_main.texPos = {0.0f,0.0f};
        cursor_main.texSize = {CURSOL_MAIN_TEX_W,CURSOL_MAIN_TEX_H};
        cursor_main.pivot   = {CURSOL_MAIN_PIVOT_X,CURSOL_MAIN_PIVOT_Y};
        cursor_main.color   = {1.0f, 1.0f, 1.0f, 1.0f};
        cursor_main_state++;

        // わざとbreakしない

    case 2:
    {
        // マウスの位置を取得
        cursor_main_posX = (float)getCursorPosX();
        cursor_main_posY = (float)getCursorPosY();

        // 取得した位置に画像を配置
        cursor_main.pos = {
            cursor_main_posX,
            cursor_main_posY
        };

        break;
    }
    }
}


//==================================================
// 描画
//==================================================

void cursor_main_render()
{
    if (cursor_main_state != 2 || sprCursor_main == nullptr)
    {
        return;
    }

    sprite_render(
        sprCursor_main,
        cursor_main_posX, cursor_main_posY,
        cursor_main.scale.x,    cursor_main.scale.y,
        cursor_main.texPos.x,  cursor_main.texPos.y,
        cursor_main.texSize.x, cursor_main.texSize.y,
        cursor_main.pivot.x,    cursor_main.pivot.y,
        ToRadian(0),
        cursor_main.color.x, cursor_main.color.y, cursor_main.color.z, cursor_main.color.w
    );
}