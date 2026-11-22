#include "cursor.h"

Sprite* sprCursor = nullptr;
OBJ2D cursor;

// 状態
int cursor_state = 0;
int cursor_timer = 0;

// マウス位置
float cursor_posX = 0.0f;
float cursor_posY = 0.0f;

void cursor_init()
{
	cursor_state = 0;
	cursor_timer = 0;
	cursor_posX = 0.0f;
	cursor_posY = 0.0f;
}

void cursor_update()
{
    switch (cursor_state)
    {
    case 0:
        // 画像読み込み
        sprCursor = sprite_load(L"./Data/Images/cursor_normal.png");

        cursor_state++;
    case 1:
        cursor = {};
        cursor.pos = { 0.0f, 0.0f };
        cursor.scale = { CURSOL_SCALE,CURSOL_SCALE };
        cursor.texPos = { 0.0f,0.0f };
        cursor.texSize = { CURSOL_TEX_W,CURSOL_TEX_H };
        cursor.pivot = { CURSOL_PIVOT_X,CURSOL_PIVOT_Y };
        cursor.color = { 1.0f, 1.0f, 1.0f, 1.0f };
        cursor_state++;

        // わざとbreakしない

    case 2:
    {
        // マウスの位置を取得
        cursor_posX = (float)getCursorPosX();
        cursor_posY = (float)getCursorPosY();

        // 取得した位置に画像を配置
        cursor.pos = {
            cursor_posX,
            cursor_posY
        };
        break;
    }
    }
}

void cursor_render()
{
    if (cursor_state != 2 || sprCursor == nullptr)
    {
        return;
    }

    sprite_render(
        sprCursor,
        cursor_posX + 20.0f,cursor_posY + 38.0f,
        cursor.scale.x,cursor.scale.y,
        cursor.texPos.x,cursor.texPos.y,
        cursor.texSize.x,cursor.texSize.y,
        cursor.pivot.x,cursor.pivot.y,
        ToRadian(0),
        cursor.color.x,cursor.color.y,cursor.color.z,cursor.color.w
    );
}

void cursor_deinit()
{
	safe_delete(sprCursor);
}
