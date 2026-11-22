#include "all.h"
#include "pause.h"
#include "result_screen.h"
#include "scene_game.h"
#include "effect_scene_change.h"
#include "Hp.h"

extern int second_timer;

Sprite* sprResult = nullptr;
Sprite* sprGameOver = nullptr;
Sprite* sprGameClear = nullptr;

int current_mode = RESULT_MODE_NONE;
int confirmed_result_mode = RESULT_MODE_NONE;

//ゲームオーバー / ゲームクリア用
float banner_pos_y = -1080.0f;
float banner_speed_y = 0.0f;
int banner_bounce_count = 0;
bool banner_landed = false;
int banner_wait_timer = 0;

//リザルト背景用
float result_pos_y = -1080.0f;
float result_speed_y = 0.0f;
float result_stop_y = 0.0f;
bool result_bg_landed = false;

int result_state = 0;
int result_bounce_count = 0;

bool result_active = false;
bool result_finished = false;
bool result_landed = false;

//BGM再生制御用
bool result_bgm_loaded = false;
bool result_bgm_played = false;

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

    current_mode = RESULT_MODE_NONE;
    confirmed_result_mode = RESULT_MODE_NONE;

    banner_pos_y = -1080.0f;
    banner_speed_y = 0.0f;
    banner_bounce_count = 0;
    banner_landed = false;
    banner_wait_timer = 0;

    result_pos_y = -1080.0f;
    result_speed_y = 0.0f;
    result_stop_y = 0.0f;
    result_bg_landed = false;

    result_bgm_loaded = false;
    result_bgm_played = false;

    if (ShowCursor(TRUE) < 0)
    {
        ShowCursor(TRUE);
    }
}

//落下＆バウンド用の共通計算
void update_drop_bounce(float& pos_y, float& speed_y, float stop_y, int& bounce_count, bool& landed)
{
    if (landed)
    {
        return;
    }

    const float gravity = 2.2f;      //重力加速度
    const float restitution = 0.45f; //跳ね返りの強さ
    const int maxBounces = 2;        //バウンド回数

    speed_y += gravity;
    pos_y += speed_y;

    if (pos_y >= stop_y)
    {
        pos_y = stop_y;

        if (bounce_count < maxBounces)
        {
            speed_y = -speed_y * restitution;
            bounce_count++;
        }
        else
        {
            speed_y = 0.0f;
            landed = true;
        }
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

    //ゲームオーバーかゲームクリアかを判定して確定値を保存
    if (confirmed_result_mode == RESULT_MODE_NONE)
    {
        if (player_hp <= 0)
        {
            player_hp = 0;
            current_mode = RESULT_MODE_GAMEOVER;
            confirmed_result_mode = RESULT_MODE_GAMEOVER;
            result_state = 0;
        }
        else if (second_timer <= 0)
        {
            second_timer = 0;
            current_mode = RESULT_MODE_GAMECLEAR;
            confirmed_result_mode = RESULT_MODE_GAMECLEAR;
            result_state = 0;
        }
    }

    int active_mode = (current_mode != RESULT_MODE_NONE) ? current_mode : confirmed_result_mode;

    switch (result_state)
    {
    case 0:
        if (sprResult == nullptr)
        {
            sprResult = sprite_load(L"Data/Images/result_back_2.png");
        }
        if (active_mode == RESULT_MODE_GAMEOVER && sprGameOver == nullptr)
        {
            sprGameOver = sprite_load(L"Data/Images/game_over.png");
        }
        if (active_mode == RESULT_MODE_GAMECLEAR && sprGameClear == nullptr)
        {
            sprGameClear = sprite_load(L"Data/Images/game_clear.png");
        }

        if (!result_bgm_loaded)
        {
            music::load(RESULT_CLEAR, L"Data/Sounds/BGM/result_clear2.wav");
            music::load(RESULT_OVER, L"Data/Sounds/BGM/result_gameover2.wav");
            result_bgm_loaded = true;
        }

        result_state++;
        break;

    case 1:
        //ゲームオーバー / ゲームクリア演出の準備
        banner_pos_y = -1080.0f;
        banner_speed_y = 28.0f;
        banner_bounce_count = 0;
        banner_landed = false;
        banner_wait_timer = 45; //バウンド後に余韻を見せるフレーム数

        //リザルト画面の準備
        result_pos_y = -1080.0f;
        result_speed_y = 28.0f;
        result_stop_y = 0.0f;
        result_bounce_count = 0;
        result_bg_landed = false;

        result_finished = false;
        result_landed = false;

        result_state++;
        break;

    case 2:
        //ゲームオーバー / ゲームクリアが降ってくる
        update_drop_bounce(banner_pos_y, banner_speed_y, 0.0f, banner_bounce_count, banner_landed);

        //文字が出た瞬間に1回だけ再生
        if (!result_bgm_played && banner_pos_y > -1080.0f && active_mode != RESULT_MODE_NONE)
        {
            music::clear();
            if (active_mode == RESULT_MODE_GAMEOVER)
            {
                music::play(RESULT_OVER, true);
            }
            else if (active_mode == RESULT_MODE_GAMECLEAR)
            {
                music::play(RESULT_CLEAR, true);
            }
            result_bgm_played = true;
        }

        if (banner_landed)
        {
            if (banner_wait_timer > 0)
            {
                banner_wait_timer--;
            }
            else
            {
                result_state++;
            }
        }
        break;

    case 3:
        //リザルト画面が降ってくる
        update_drop_bounce(result_pos_y, result_speed_y, result_stop_y, result_bounce_count, result_bg_landed);

        if (result_bg_landed)
        {
            result_landed = true;
            result_finished = true;
            nextScene = SCENE_RESULT;
            result_state++;
        }
        break;

    case 4:
        // 完了待機
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

    int active_mode = (current_mode != RESULT_MODE_NONE) ? current_mode : confirmed_result_mode;

    //ゲームオーバー / ゲームクリア画像
    Sprite* active_banner = nullptr;
    if (active_mode == RESULT_MODE_GAMEOVER)
    {
        active_banner = sprGameOver;
    }
    else if (active_mode == RESULT_MODE_GAMECLEAR)
    {
        active_banner = sprGameClear;
    }

    if (active_banner != nullptr)
    {
        sprite_render(
            active_banner,
            0.0f, banner_pos_y,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1920.0f, 1080.0f,
            0.0f, 0.0f,
            ToRadian(0.0f),
            1.0f, 1.0f, 1.0f, 1.0f
        );
    }

    //リザルト背景
    if (result_state >= 3 && sprResult != nullptr)
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
    if (sprGameOver != nullptr)
    {
        safe_delete(sprGameOver);
        sprGameOver = nullptr;
    }
    if (sprGameClear != nullptr)
    {
        safe_delete(sprGameClear);
        sprGameClear = nullptr;
    }

    current_mode = RESULT_MODE_NONE;
    result_state = 0;
    result_bounce_count = 0;
    result_active = false;
    result_finished = false;
    result_landed = false;
    result_bg_landed = false;
    banner_landed = false;
    result_bgm_played = false;
}