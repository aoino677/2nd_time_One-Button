#include "all.h"
#include "pause.h"
#include "result_screen.h"
#include "scene_game.h"
#include "effect_scene_change.h"

Sprite* sprResult = nullptr;

float result_pos_y = -1080.0f;
float result_speed_y = 0.0f;
float result_stop_y = 0.0f;

int result_state = 0;
int result_bounce_count = 0;

bool result_active = false;
bool result_finished = false;
bool result_landed = false;

//------------------------------------------------------------
// 結果画面ボタン
//------------------------------------------------------------
OBJ2D resultButton[2];

Sprite* sprResultButton[2] = { nullptr, nullptr };
Sprite* sprResultScore = nullptr;
Sprite* sprKousin = nullptr;

//------------------------------------------------------------
// 初期化
//------------------------------------------------------------
void result_screen_init()
{
    result_state = 0;
    result_bounce_count = 0;
    result_active = false;
    result_finished = false;
    result_landed = false;

    //リザルト画面を画面外の初期位置に
    result_pos_y = -1080.0f;
    result_speed_y = 0.0f;
    result_stop_y = 0.0f;

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
void result_screen_update()
{
    if (!result_active)
    {
        return;
    }

    switch (result_state)
    {
    case 0:
        //リザルト画面
        if (sprResult == nullptr)
        {
            sprResult = sprite_load(L"Data/Images/result_screen.png");
        }

        //RETRY/TITLE 画像
        if (sprResultButton[0] == nullptr)
        {
            sprResultButton[0] = sprite_load(L"Data/Images/retry.png");
        }
        if (sprResultButton[1] == nullptr)
        {
            sprResultButton[1] = sprite_load(L"Data/Images/title_sin.png");
        }

        // -------------------------
        // RETRY
        // -------------------------
        resultButton[0] = {};
        resultButton[0].pos = { 500.0f, 850.0f };
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

        // -------------------------
        // TITLE
        // -------------------------
        resultButton[1] = {};
        resultButton[1].pos = { 1500.0f, 850.0f };
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

        result_state++;
        break;

    case 1:
        //落下・2回バウンドの準備
        result_pos_y = -1080.0f;
        result_speed_y = 28.0f;
        result_stop_y = 0.0f;
        result_bounce_count = 0;
        result_finished = false;
        result_landed = false;

        result_state++;
        break;

    case 2:
        //落下＆2回バウンド処理
        if (!result_landed)
        {
            const float gravity = 2.2f;       //重力加速度
            const float restitution = 0.45f;  //跳ね返りの強さ
            const int maxBounces = 2;         //バウンドする回数

            result_speed_y += gravity;
            result_pos_y += result_speed_y;

            //0.0fに到達したら着地・バウンド判定
            if (result_pos_y >= result_stop_y)
            {
                result_pos_y = result_stop_y;

                if (result_bounce_count < maxBounces)
                {
                    //上向きに跳ね返る
                    result_speed_y = -result_speed_y * restitution;
                    result_bounce_count++;
                }
                else
                {
                    //2回バウンドし終えたらピタッと停止
                    result_speed_y = 0.0f;
                    result_landed = true;
                }
            }
        }

        // -------------------------
        // ボタン判定
        // -------------------------
        if (result_landed)
        {
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
                        switch (i)
                        {
                        case 0:
                            //RETRY
                            result_active = false;
                            result_finished = true;
                            result_landed = false;
                            result_bounce_count = 0;
                            result_state = 1;
                            scene_change_init();
                            game_init();
                            break;

                        case 1:
                            //TITLE
                            result_active = false;
                            result_finished = true;
                            result_landed = false;
                            result_bounce_count = 0;
                            result_state = 1;
                            nextScene = SCENE_TITLE;
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
        }

        break;
    }
}

//------------------------------------------------------------
// 描画
//------------------------------------------------------------
void result_screen_render()
{
    if (!result_active)
    {
        return;
    }

    //リザルト画面
    if (sprResult != nullptr)
    {
        sprite_render(
            sprResult,
            0.0f, result_pos_y,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1920.0f, 1080.0f,
            0.0f, 0.0f,
            ToRadian(0.0f),
            1.0f, 1.0f, 1.0f, 1.0f
        );
    }

    //完全停止後にボタンを描画
    if (result_landed)
    {
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
                    resultButton[i].color.w
                );
            }
        }
    }
}

//------------------------------------------------------------
// 終了判定
//------------------------------------------------------------
bool result_screen_is_finished()
{
    return result_finished;
}

//------------------------------------------------------------
// 終了処理
//------------------------------------------------------------
void result_screen_deinit()
{
    if (sprResult != nullptr)
    {
        safe_delete(sprResult);
        sprResult = nullptr;
    }

    for (int i = 0; i < 2; i++)
    {
        if (sprResultButton[i] != nullptr)
        {
            safe_delete(sprResultButton[i]);
            sprResultButton[i] = nullptr;
        }
    }

    result_state = 0;
    result_bounce_count = 0;
    result_active = false;
    result_landed = false;
}