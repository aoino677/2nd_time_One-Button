#include "all.h"
#include "pause.h"
#include "result_button.h"
#include "result_screen.h"
#include "scene_game.h"
#include "scene_title.h"
#include "effect_scene_change.h"

int result_button_state = 0;

//------------------------------------------------------------
// 結果画面ボタン
//------------------------------------------------------------
OBJ2D resultButton[2];

Sprite* sprResultButton[2] = { nullptr, nullptr };

//------------------------------------------------------------
// 初期化
//------------------------------------------------------------
void result_button_init()
{
    result_button_state = 0;
    result_active = false;
    result_finished = false;
    result_landed = false;

    for (int i = 0; i < 2; i++)
    {
        resultButton[i] = {};
    }

    while (ShowCursor(TRUE) < 0)
    {
    }
}

//------------------------------------------------------------
// 更新
//------------------------------------------------------------
void result_button_update()
{
     //コイン演出中はボタン入力を受け付けない
    if (is_scene_changing())
    {
        return;
    }

    switch (result_button_state)
    {
    case 0:
        //RETRY/TITLE 画像
        if (sprResultButton[0] == nullptr)
        {
            sprResultButton[0] = sprite_load(L"Data/Images/retry.png");
        }
        if (sprResultButton[1] == nullptr)
        {
            sprResultButton[1] = sprite_load(L"Data/Images/title_sin.png");
        }

        //-------------------------
        //RETRY
        //-------------------------
        resultButton[0] = {};
        resultButton[0].pos = { 500.0f, 950.0f };
        resultButton[0].scale = {
            static_cast<float>(PAUSE_SCALE),
            static_cast<float>(PAUSE_SCALE)
        };
        resultButton[0].texPos = { 0.0f, 0.0f };
        resultButton[0].texSize = {
            static_cast<float>(PAUSE_TEX_W),
            static_cast<float>(PAUSE_TEX_H)
        };
        resultButton[0].pivot = {
            static_cast<float>(PAUSE_PIVOT_X),
            static_cast<float>(PAUSE_PIVOT_Y)
        };
        resultButton[0].color = { 1.0f, 1.0f, 1.0f, 1.0f };

        //-------------------------
        //TITLE
        //-------------------------
        resultButton[1] = {};
        resultButton[1].pos = { 1500.0f, 950.0f };
        resultButton[1].scale = {
            static_cast<float>(PAUSE_SCALE),
            static_cast<float>(PAUSE_SCALE)
        };
        resultButton[1].texPos = { 0.0f, 0.0f };
        resultButton[1].texSize = {
            static_cast<float>(PAUSE_TEX_W),
            static_cast<float>(PAUSE_TEX_H)
        };
        resultButton[1].pivot = {
            static_cast<float>(PAUSE_PIVOT_X),
            static_cast<float>(PAUSE_PIVOT_Y)
        };
        resultButton[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };

        result_button_state++;
        break;

    case 1:
        //-------------------------
        //ボタン判定
        //-------------------------

        float mouseX = static_cast<float>(getCursorPosX());
        float mouseY = static_cast<float>(getCursorPosY());

        for (int i = 0; i < 2; i++)
        {
            float left =
                resultButton[i].pos.x -
                resultButton[i].pivot.x * resultButton[i].scale.x;

            float right =
                resultButton[i].pos.x +
                (resultButton[i].texSize.x - resultButton[i].pivot.x) * resultButton[i].scale.x;

            float top =
                resultButton[i].pos.y -
                resultButton[i].pivot.y * resultButton[i].scale.y;

            float bottom =
                resultButton[i].pos.y +
                (resultButton[i].texSize.y - resultButton[i].pivot.y) * resultButton[i].scale.y;

            if (mouseX >= left &&
                mouseX <= right &&
                mouseY >= top &&
                mouseY <= bottom)
            {
                //マウスを乗せたら少し拡大
                resultButton[i].scale.x = static_cast<float>(PAUSE_SCALE) + 0.1f;
                resultButton[i].scale.y = static_cast<float>(PAUSE_SCALE) + 0.1f;

                if (TRG(0) & PAD_START)
                {
                    result_active = false;
                    result_finished = true;
                    result_landed = false;

                    switch (i)
                    {
                    case 0:
                        //RETRY
                        scene_change_start(SCENE_GAME);
                        break;

                    case 1:
                        //TITLE
                        scene_change_start(SCENE_TITLE);
                        break;
                    }
                    break;
                }
            }
            else
            {
                //通常サイズに戻す
                resultButton[i].scale.x = static_cast<float>(PAUSE_SCALE);
                resultButton[i].scale.y = static_cast<float>(PAUSE_SCALE);
            }
        }
        break;
    }
}

//------------------------------------------------------------
// 描画
//------------------------------------------------------------
void result_button_render()
{
    //完全停止後にボタンを描画
    for (int i = 0; i < 2; i++)
    {
        if (sprResultButton[i] != nullptr)
        {
            sprite_render(
                sprResultButton[i],
                resultButton[i].pos.x,
                resultButton[i].pos.y,
                resultButton[i].scale.x,
                resultButton[i].scale.y,
                resultButton[i].texPos.x,
                resultButton[i].texPos.y,
                resultButton[i].texSize.x,
                resultButton[i].texSize.y,
                resultButton[i].pivot.x,
                resultButton[i].pivot.y,
                ToRadian(0.0f),
                resultButton[i].color.x,
                resultButton[i].color.y,
                resultButton[i].color.z,
                resultButton[i].color.w);
        }
    }
}

//------------------------------------------------------------
// 終了処理
//------------------------------------------------------------
void result_button_deinit()
{
    for (int i = 0; i < 2; i++)
    {
        if (sprResultButton[i] != nullptr)
        {
            safe_delete(sprResultButton[i]);
            sprResultButton[i] = nullptr;
        }
    }
}