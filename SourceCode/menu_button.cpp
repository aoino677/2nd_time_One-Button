#include "obj2d.h"
#include "menu_button.h"
#include "scene_menu.h"
#include "cursor_main.h"
#include "effect_start_game.h"

//--------------------------------------
// メニューボタン
//--------------------------------------
OBJ2D menuButton;

Sprite* sprMenuButton = nullptr;

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

    menuButton = {};
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
        sprMenuButton = sprite_load(L"./Data/Images/NewGame_sin.png");

        menu_button_state++;
        break;

    case 1:
        //ゲームスタートボタン初期化
        menuButton = {};

        menuButton.pos = { 1400.0f, 650.0f };

        menuButton.scale =
        {
            MENU_BUTTON_SCALE,
            MENU_BUTTON_SCALE
        };

        menuButton.texPos = { 0.0f, 0.0f };

        menuButton.texSize =
        {
            MENU_BUTTON_TEX_W,
            MENU_BUTTON_TEX_H
        };

        menuButton.pivot =
        {
            MENU_BUTTON_PIVOT_X,
            MENU_BUTTON_PIVOT_Y
        };

        menuButton.color = { 1.0f, 1.0f, 1.0f, 1.0f };

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
        float left =
            menuButton.pos.x -
            menuButton.pivot.x * menuButton.scale.x;

        float right =
            left + menuButton.texSize.x * menuButton.scale.x;

        float top =
            menuButton.pos.y -
            menuButton.pivot.y * menuButton.scale.y;

        float bottom =
            top + menuButton.texSize.y * menuButton.scale.y;

        if (mouseX >= left &&
            mouseX <= right &&
            mouseY >= top &&
            mouseY <= bottom)
        {
            //マウスを乗せると少し拡大
            menuButton.scale.x = MENU_BUTTON_SCALE + 0.1f;
            menuButton.scale.y = MENU_BUTTON_SCALE + 0.1f;

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
            menuButton.scale.x = MENU_BUTTON_SCALE;
            menuButton.scale.y = MENU_BUTTON_SCALE;
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
    if (sprMenuButton != nullptr)
    {
        sprite_render(
            sprMenuButton,
            menuButton.pos.x,
            menuButton.pos.y,
            menuButton.scale.x,
            menuButton.scale.y,
            menuButton.texPos.x,
            menuButton.texPos.y,
            menuButton.texSize.x,
            menuButton.texSize.y,
            menuButton.pivot.x,
            menuButton.pivot.y,
            ToRadian(0),
            menuButton.color.x,
            menuButton.color.y,
            menuButton.color.z,
            menuButton.color.w
        );
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
    safe_delete(sprMenuButton);

    menu_button_state = 0;
    menu_button_timer = 0;
    menu_wipe_closing = false;
}