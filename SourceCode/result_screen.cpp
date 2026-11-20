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
extern int result_bounce_count = 0;

extern bool result_active = false;
extern bool result_finished = false;
extern bool result_landed = false;

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
                    nextScene = SCENE_RESULT;
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

    result_state = 0;
    result_bounce_count = 0;
    result_active = false;
    result_landed = false;
}