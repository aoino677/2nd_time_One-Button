#include "all.h"
#include "scene_game.h"
#include "pause.h"

int pause_state = 0;
int pause_timer = 0;

int effect_retry_timer = 0;
bool retry_fadein = false;

//--------------------------------------
// ポーズ画面の3つのボタン
//--------------------------------------
OBJ2D pauseButton[3];
Sprite* sprPauseButton[3] = {};

//--------------------------------------
// 右下のポーズボタン
//--------------------------------------
OBJ2D pauseIcon;
Sprite* sprPauseIcon = nullptr;

//--------------------------------------
// ポーズ画面の特殊演出
//--------------------------------------
float pause_effect_y = 0.0f;
float pause_effect_alpha = 0.0f;

//--------------------------------------
// 初期化
//--------------------------------------
void pause_init()
{
    pause_state = 0;
    pause_timer = 0;
    retry_fadein = false;

    pause_effect_y = 0.0f;
    pause_effect_alpha = 0.0f;

    //----------------------------------
    // 右下のポーズボタン画像
    //----------------------------------
    sprPauseIcon = sprite_load(L"./Data/Images/pause_button.png");
    //----------------------------------
    // ポーズ画面のボタン画像
    //----------------------------------
    sprPauseButton[0] = sprite_load(L"./Data/Images/game_back_sin.png");
    sprPauseButton[1] = sprite_load(L"./Data/Images/retry.png");
    sprPauseButton[2] = sprite_load(L"./Data/Images/title_sin.png");
    //----------------------------------
    // 右下のポーズボタン
    //----------------------------------
    pauseIcon = {};
    pauseIcon.pos = { static_cast<float>(PAUSEBUTTON_POS_X),static_cast<float>(PAUSEBUTTON_POS_Y) };
    pauseIcon.scale = { static_cast<float>(PAUSEBUTTON_SCALE),static_cast<float>(PAUSEBUTTON_SCALE) };
    pauseIcon.texSize = { static_cast<float>(PAUSEBUTTON_TEX_W),static_cast<float>(PAUSEBUTTON_TEX_H) };
    pauseIcon.pivot = { static_cast<float>(PAUSEBUTTON_PIVOT_X),static_cast<float>(PAUSEBUTTON_PIVOT_Y) };
    pauseIcon.color = { 1.0f, 1.0f, 1.0f, 1.0f };
    //----------------------------------
    // CONTINUE
    //----------------------------------
    pauseButton[0]           = {};
    pauseButton[0].pos      = { 1000.0f, 400.0f };
    pauseButton[0].scale    = { static_cast<float>(PAUSE_SCALE),static_cast<float>(PAUSE_SCALE) };
    pauseButton[0].texSize  = { static_cast<float>(PAUSE_TEX_W),static_cast<float>(PAUSE_TEX_H) };
    pauseButton[0].pivot    = { static_cast<float>(PAUSE_PIVOT_X),static_cast<float>(PAUSE_PIVOT_Y) };
    pauseButton[0].color    = { 1.0f, 1.0f, 1.0f, 1.0f };
    //----------------------------------
    // RESTART
    //----------------------------------
    pauseButton[1] = {};
    pauseButton[1].pos = { 1000.0f, 600.0f };
    pauseButton[1].scale = { static_cast<float>(PAUSE_SCALE),static_cast<float>(PAUSE_SCALE) };
    pauseButton[1].texSize = { static_cast<float>(PAUSE_TEX_W),static_cast<float>(PAUSE_TEX_H) };
    pauseButton[1].pivot = { static_cast<float>(PAUSE_PIVOT_X),static_cast<float>(PAUSE_PIVOT_Y) };
    pauseButton[1].color = { 1.0f, 1.0f, 1.0f, 1.0f };
    //----------------------------------
    // TITLE
    //----------------------------------
    pauseButton[2] = {};
    pauseButton[2].pos = { 1000.0f, 800.0f };
    pauseButton[2].scale = { static_cast<float>(PAUSE_SCALE),static_cast<float>(PAUSE_SCALE) };
    pauseButton[2].texSize = { static_cast<float>(PAUSE_TEX_W),static_cast<float>(PAUSE_TEX_H) };
    pauseButton[2].pivot = { static_cast<float>(PAUSE_PIVOT_X),static_cast<float>(PAUSE_PIVOT_Y) };
    pauseButton[2].color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

//--------------------------------------
// 更新
//--------------------------------------
void pause_update()
{
    //----------------------------------
    // 通常ゲーム中
    //----------------------------------
    if (!pause)
    {
        float mouseX = static_cast<float>(getCursorPosX());
        float mouseY = static_cast<float>(getCursorPosY());

        float left = pauseIcon.pos.x - pauseIcon.pivot.x * PAUSEBUTTON_SCALE;
        float right = pauseIcon.pos.x + (pauseIcon.texSize.x - pauseIcon.pivot.x) * PAUSEBUTTON_SCALE;
        float top = pauseIcon.pos.y - pauseIcon.pivot.y * PAUSEBUTTON_SCALE;
        float bottom = pauseIcon.pos.y + (pauseIcon.texSize.y - pauseIcon.pivot.y) * PAUSEBUTTON_SCALE;
        //----------------------------------
        // ポーズボタンの判定
        //----------------------------------
        if (mouseX >= left &&
            mouseX <= right &&
            mouseY >= top &&
            mouseY <= bottom)
        {
            pauseIcon.scale.x = PAUSEBUTTON_SCALE + 0.2f;
            pauseIcon.scale.y = PAUSEBUTTON_SCALE + 0.2f;

            if (TRG(0) & PAD_START)
            {
                pause = true;

                pause_effect_y = static_cast<float>(SCREEN_H);
                pause_effect_alpha = 0.0f;

                pause_timer = 0;
                pause_state = 0;
            }
        }
        else
        {
            pauseIcon.scale.x = PAUSEBUTTON_SCALE;
            pauseIcon.scale.y = PAUSEBUTTON_SCALE;
        }
        return;
    }

    //----------------------------------
    // ポーズ中
    //----------------------------------
    switch (pause_state)
    {
        //----------------------------------
        // 演出開始
        //----------------------------------
    case 0:
        pause_effect_y = static_cast<float>(SCREEN_H);
        pause_effect_alpha = 0.0f;
        pause_timer = 0;

        pause_state++;
        break;

        //----------------------------------
        // ポーズ画面の演出
        //----------------------------------
    case 1:
        pause_timer++;

        // 背景を徐々に暗くする
        pause_effect_alpha += 0.04f;

        if (pause_effect_alpha >= 0.8f)
        {
            pause_effect_alpha = 0.8f;
        }

        // ボタンを下から上へ移動
        pause_effect_y -= 40.0f;

        if (pause_effect_y <= 0.0f)
        {
            pause_effect_y = 0.0f;
        }

        // 演出終了
        if (pause_effect_y <= 0.0f &&
            pause_effect_alpha >= 0.8f)
        {
            pause_state++;
        }

        break;

        //----------------------------------
        // ボタン操作
        //----------------------------------
    case 2:
        for (int i = 0; i < 3; i++)
        {
            float mouseX = static_cast<float>(getCursorPosX());
            float mouseY = static_cast<float>(getCursorPosY());

            float buttonY =pauseButton[i].pos.y + pause_effect_y;
            float left =pauseButton[i].pos.x -pauseButton[i].pivot.x *pauseButton[i].scale.x;
            float right =pauseButton[i].pos.x +(pauseButton[i].texSize.x -pauseButton[i].pivot.x) *pauseButton[i].scale.x;
            float top =buttonY -pauseButton[i].pivot.y *pauseButton[i].scale.y;
            float bottom =buttonY +(pauseButton[i].texSize.y -pauseButton[i].pivot.y) *pauseButton[i].scale.y; 
            //----------------------------------
            // マウスがボタンの中にある
            //----------------------------------
            if (mouseX >= left &&
                mouseX <= right &&
                mouseY >= top &&
                mouseY <= bottom)
            {
                pauseButton[i].scale.x = PAUSE_SCALE + 0.1f;
                pauseButton[i].scale.y = PAUSE_SCALE + 0.1f;
                //----------------------------------
                // クリック
                //----------------------------------
                if (TRG(0) & PAD_START)
                {
                    switch (i)
                    {
                        //----------------------------------
                        // CONTINUE
                        //----------------------------------
                    case 0:
                        pause = false;
               
                        pause_state = 0;
                        pause_effect_y = 0.0f;
                        pause_effect_alpha = 0.0f;
                        break;

                        //----------------------------------
                        // RESTART
                        //----------------------------------
                    case 1:
                        game_deinit();
                        game_init();

                        pause = false;
                        pause_state = 0;
                        pause_effect_y = 0.0f;
                        pause_effect_alpha = 0.0f;

                        break;
                        //----------------------------------
                        // TITLE
                        //----------------------------------
                    case 2:
                        nextScene = SCENE_TITLE;
                        break;
                    }
                }
            }
            else
            {
                pauseButton[i].scale.x = PAUSE_SCALE;
                pauseButton[i].scale.y = PAUSE_SCALE;
            }
        }
        break;
    }
}

//--------------------------------------
// 描画
//--------------------------------------
void pause_render()
{
    //----------------------------------
    // 通常ゲーム中
    //----------------------------------
    if (!pause)
    {
        if (sprPauseIcon != nullptr)
        {
            sprite_render(
                sprPauseIcon,
                pauseIcon.pos.x,
                pauseIcon.pos.y,
                pauseIcon.scale.x,
                pauseIcon.scale.y,
                pauseIcon.texPos.x,
                pauseIcon.texPos.y,
                pauseIcon.texSize.x,
                pauseIcon.texSize.y,
                pauseIcon.pivot.x,
                pauseIcon.pivot.y,
                ToRadian(0.0f),
                pauseIcon.color.x,
                pauseIcon.color.y,
                pauseIcon.color.z,
                pauseIcon.color.w
            );
        }
        return;
    }

    //----------------------------------
    // 暗い背景
    //----------------------------------
    primitive::rect(
        0.0f, 0.0f,
        static_cast<float>(SCREEN_W),
        static_cast<float>(SCREEN_H),
        0.0f, 0.0f,
        0.0f,
        0.0f, 0.0f, 0.0f,
        pause_effect_alpha
    );

    //----------------------------------
    // 3つのボタン
    //----------------------------------
    for (int i = 0; i < 3; i++)
    {
        if (sprPauseButton[i] == nullptr)
            continue;

        sprite_render(
            sprPauseButton[i],
            pauseButton[i].pos.x,     pauseButton[i].pos.y + pause_effect_y,
            pauseButton[i].scale.x,   pauseButton[i].scale.y,
            pauseButton[i].texPos.x, pauseButton[i].texPos.y,
            pauseButton[i].texSize.x ,pauseButton[i].texSize.y,
            pauseButton[i].pivot.x,   pauseButton[i].pivot.y,
            ToRadian(0.0f),
            pauseButton[i].color.x,pauseButton[i].color.y,pauseButton[i].color.z,pauseButton[i].color.w
        );
    }
}

//--------------------------------------
// 終了処理
//--------------------------------------
void pause_deinit()
{
    // ポーズアイコン削除
    safe_delete(sprPauseIcon);

    // ポーズボタン削除
    for (int i = 0; i < 3; i++)
    {
        safe_delete(sprPauseButton[i]);
    }
}

