#include "all.h"
#include "obj2d.h"
#include "cursor_main.h"
#include "enemy.h"
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
int timer_down;
int timer = 0;

// 武器選択値
int serect_weapon;

// カーソルの当たり判定サイズ
float cursor_hitbox_half_width = 10.0f;
float cursor_hitbox_half_height = 10.0f;

// 通常時のスケール
float cursor_normal_scale = CURSOL_MAIN_SCALE;

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

    timer_down = TIMER;
    timer = timer_down;

    serect_weapon = 6;

    cursor_main = {};

    cursor_normal_scale = CURSOL_MAIN_SCALE;

    cursor_hitbox_half_width = 10.0f;
    cursor_hitbox_half_height = 10.0f;
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

        if (timer > timer_down)
        {
            timer = timer_down;
        }
        for (int i = 0; i < ENEMY_NUMBER; i++)
        {
            // 敵の当たり判定
            float enemy_left = enemy[i].pos.x - enemy[i].pivot.x * ENEMY_SCALE;
            float enemy_right = enemy[i].pos.x + (enemy[i].texSize.x - enemy[i].pivot.x) * ENEMY_SCALE;
            float enemy_top = enemy[i].pos.y - enemy[i].pivot.y * ENEMY_SCALE;
            float enemy_bottom = enemy[i].pos.y + (enemy[i].texSize.y - enemy[i].pivot.y) * ENEMY_SCALE;

            // カーソル中央の当たり判定
            float cursor_left = cursor_main_posX - cursor_hitbox_half_width;
            float cursor_right = cursor_main_posX + cursor_hitbox_half_width;
            float cursor_top = cursor_main_posY - cursor_hitbox_half_height;
            float cursor_bottom = cursor_main_posY + cursor_hitbox_half_height;

            // 敵とカーソルの衝突判定
            if (cursor_left <= enemy_right &&
                cursor_right >= enemy_left &&
                cursor_top <= enemy_bottom &&
                cursor_bottom >= enemy_top)
            {
                if (timer == timer_down)
                {
                    switch (serect_weapon)
                    {
                    case 6:
                        shot_bullet();
                        break;

                    case 3:
                        shot_bullet_Ingot();
                        break;

                    case 1:
                        shot_bullet_Shell();
                        break;

                    case 2:
                        shot_bullet_Beam();
                        break;

                    case 4:
                        shot_bullet_Bankroll();
                        break;
                    }

                    timer--;
                }
                else if (timer == 0)
                {
                    timer = timer_down;
                }
                else
                {
                    timer--;
                }

                break;
            }
        }
      
        

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

// カーソルを大きくして、その分当たり判定も大きくする
void Cursor_big()
{
    // 1回呼び出すごとに少し大きくする
    cursor_main.scale.x += 0.1f;
    cursor_main.scale.y += 0.1f;

    // 当たり判定も少し大きくする
    cursor_hitbox_half_width += 2.0f;
    cursor_hitbox_half_height += 2.0f;
}
