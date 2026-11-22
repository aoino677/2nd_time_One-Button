#include "obj2d.h"
#include "menu_button.h"
#include "scene_menu.h"
#include "cursor_main.h"
#include "effect_start_game.h"

//--------------------------------------
// メニューボタン
//--------------------------------------
#define MENU_BUTTON_COUNT 4
OBJ2D menuButton[MENU_BUTTON_COUNT];

Sprite* sprmenuButton[MENU_BUTTON_COUNT] = { nullptr };

int menu_button_state = 0;
int menu_button_timer = 0;
bool menu_wipe_closing = false; // 扉を閉じてシーン遷移待ち中かどうか

extern bool gameEnd;


//初期化
void menu_button_init()
{
    menu_button_state = 0;
    menu_button_timer = 0;
    menu_wipe_closing = false;

    for(int i = 0; i < MENU_BUTTON_COUNT; i++)
    menuButton[i] = {};
}

//--------------------------------------
// 更新
//--------------------------------------
void menu_button_update()
{
    switch (menu_button_state)
    {
    case 0:
        //ボタン画像読み込み
        sprmenuButton[0] = sprite_load(L"./Data/Images/tutorial.png");
        sprmenuButton[1] = sprite_load(L"./Data/Images/easy.png");
        sprmenuButton[2] = sprite_load(L"./Data/Images/normal.png");
        sprmenuButton[3] = sprite_load(L"./Data/Images/hard.png");

        menu_button_state++;
        break;

    case 1:
        //ゲームスタートボタン初期化
        for (int i = 0; i < MENU_BUTTON_COUNT; i++)
        {
            menuButton[i] = {};

            menuButton[i].pos = { static_cast<float>(SCREEN_W) * (0.2f + 0.2f * i), static_cast<float>(SCREEN_H) * 0.8f };

            menuButton[i].scale =
            {
                MENU_BUTTON_SCALE,
                MENU_BUTTON_SCALE
            };

            menuButton[i].texPos = { 0.0f, 0.0f };

            menuButton[i].texSize =
            {
                MENU_BUTTON_TEX_W,
                MENU_BUTTON_TEX_H
            };

            menuButton[i].pivot =
            {
                MENU_BUTTON_PIVOT_X,
                MENU_BUTTON_PIVOT_Y
            };

            menuButton[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };

        }
        menu_button_state++;
        break;

    case 2:
    {
        // すでに扉を閉じている最中なら、閉じきったかだけ監視する
        if (menu_wipe_closing)
        {
            scene_wipe_in_update();

            if (is_scene_wipe_in_finished())
            {
                nextScene = SCENE_GAME;
            }
            break;
        }

        //----------------------------------
        // マウス座標
        //----------------------------------
        float mouseX = static_cast<float>(getCursorPosX());
        float mouseY = static_cast<float>(getCursorPosY());

        //----------------------------------
        // ゲームスタートボタンの当たり判定
        //----------------------------------
        for (int i = 0; i < MENU_BUTTON_COUNT; i++)
        {
            float left =
                menuButton[i].pos.x -
                menuButton[i].pivot.x * menuButton[i].scale.x;

            float right =
                left + menuButton[i].texSize.x * menuButton[i].scale.x;

            float top =
                menuButton[i].pos.y -
                menuButton[i].pivot.y * menuButton[i].scale.y;

            float bottom =
                top + menuButton[i].texSize.y * menuButton[i].scale.y;

            if (mouseX >= left &&
                mouseX <= right &&
                mouseY >= top &&
                mouseY <= bottom)
            {
                //マウスを乗せると少し拡大
                menuButton[i].scale.x = MENU_BUTTON_SCALE + 0.1f;
                menuButton[i].scale.y = MENU_BUTTON_SCALE + 0.1f;

                //クリックで扉を閉じてからゲーム開始
                if (TRG(0) & PAD_START)
                {
                    menu_wipe_closing = true;
                    scene_wipe_in_start();
                }
            }
            else
            {
                //通常サイズ
                menuButton[i].scale.x = MENU_BUTTON_SCALE;
                menuButton[i].scale.y = MENU_BUTTON_SCALE;
            }
        }
        menu_button_timer = 0;
        break;
    }
    }
}

//--------------------------------------
// 描画
//--------------------------------------
void menu_button_render()
{
    //----------------------------------
    // ゲームスタートボタン
    //----------------------------------

    for (int i = 0; i < MENU_BUTTON_COUNT; i++)
    {
        if (sprmenuButton[i] != nullptr)
        {
            sprite_render(
                sprmenuButton[i],
                menuButton[i].pos.x,
                menuButton[i].pos.y,
                menuButton[i].scale.x,
                menuButton[i].scale.y,
                menuButton[i].texPos.x,
                menuButton[i].texPos.y,
                menuButton[i].texSize.x,
                menuButton[i].texSize.y,
                menuButton[i].pivot.x,
                menuButton[i].pivot.y,
                ToRadian(0),
                menuButton[i].color.x,
                menuButton[i].color.y,
                menuButton[i].color.z,
                menuButton[i].color.w
            );
        }
    }

    // 閉じている最中はボタンの上に描画する
    if (menu_wipe_closing)
    {
        scene_wipe_in_render();
    }
}

//--------------------------------------
// 終了処理
//--------------------------------------
void menu_button_deinit()
{
    for (int i = 0; i < MENU_BUTTON_COUNT; i++)
    {
        safe_delete(sprmenuButton[i]);
    }

    menu_button_state = 0;
    menu_button_timer = 0;
    menu_wipe_closing = false;
}