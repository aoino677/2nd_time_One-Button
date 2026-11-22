#include "obj2d.h"
#include "title_button.h"
#include "scene_title.h"
#include "cursor_main.h"
#include "effect_scene_change.h"

//--------------------------------------
// メニューボタン
//--------------------------------------
OBJ2D titleButton;

Sprite* sprtitleButton = nullptr;

int title_button_state = 0;
int title_button_timer = 0;

extern bool gameEnd;

//初期化
void title_button_init()
{
    title_button_state = 0;
    title_button_timer = 0;

    if (sprtitleButton != nullptr)
    {
        safe_delete(sprtitleButton);
        sprtitleButton = nullptr;
    }

    titleButton = {};
}

//--------------------------------------
// 更新
//--------------------------------------
void title_button_update()
{
    switch (title_button_state)
    {
    case 0:

        //ボタン画像読み込み
        sprtitleButton = sprite_load(L"./Data/Images/NewGame_sin.png");

        title_button_state++;
        break;

    case 1:
        //ゲームスタートボタン初期化
        titleButton = {};

        titleButton.pos = { 1300.0f, 750.0f };

        titleButton.scale =
        {
            TITLE_BUTTON_SCALE,
            TITLE_BUTTON_SCALE
        };

        titleButton.texPos = { 0.0f, 0.0f };

        titleButton.texSize =
        {
            TITLE_BUTTON_TEX_W,
            TITLE_BUTTON_TEX_H
        };

        titleButton.pivot =
        {
            TITLE_BUTTON_PIVOT_X,
            TITLE_BUTTON_PIVOT_Y
        };

        titleButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

        title_button_state++;
        break;

    case 2:

        //----------------------------------
        // マウス座標
        //----------------------------------
        float mouseX = static_cast<float>(getCursorPosX());
        float mouseY = static_cast<float>(getCursorPosY());

        //----------------------------------
        // ゲームスタートボタンの当たり判定
        //----------------------------------
        float left =
            titleButton.pos.x -
            titleButton.pivot.x * titleButton.scale.x;

        float right =
            left + titleButton.texSize.x * titleButton.scale.x;

        float top =
            titleButton.pos.y -
            titleButton.pivot.y * titleButton.scale.y;

        float bottom =
            top + titleButton.texSize.y * titleButton.scale.y;

        if (mouseX >= left &&
            mouseX <= right &&
            mouseY >= top &&
            mouseY <= bottom)
        {
            //マウスを乗せると少し拡大
            titleButton.scale.x = TITLE_BUTTON_SCALE + 0.1f;
            titleButton.scale.y = TITLE_BUTTON_SCALE + 0.1f;

            //クリックでコイン演出つきでSCENE_MENUへ遷移開始
            if (TRG(0) & PAD_START)
                scene_change_start(SCENE_MENU);
            
        }
        else
        {
            //通常サイズ
            titleButton.scale.x = TITLE_BUTTON_SCALE;
            titleButton.scale.y = TITLE_BUTTON_SCALE;
        }

        title_button_timer = 0;
        break;

    }
}

//--------------------------------------
// 描画
//--------------------------------------
void title_button_render()
{
    //----------------------------------
    // ゲームスタートボタン
    //----------------------------------
    if (sprtitleButton != nullptr)
    {
        sprite_render(
            sprtitleButton,
            titleButton.pos.x,
            titleButton.pos.y,
            titleButton.scale.x,
            titleButton.scale.y,
            titleButton.texPos.x,
            titleButton.texPos.y,
            titleButton.texSize.x,
            titleButton.texSize.y,
            titleButton.pivot.x,
            titleButton.pivot.y,
            ToRadian(0),
            titleButton.color.x,
            titleButton.color.y,
            titleButton.color.z,
            titleButton.color.w
        );
    }
}

//--------------------------------------
// 終了処理
//--------------------------------------
// 終了処理
void title_button_deinit()
{
    if (sprtitleButton != nullptr)
    {
        safe_delete(sprtitleButton);
        sprtitleButton = nullptr;
    }

    title_button_state = 0;
    title_button_timer = 0;
}