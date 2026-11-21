#include "all.h"
#include "obj2d.h"
#include "cursor_main.h"
#include "enemy.h"
#include "player.h"
#include "Level.h"
#include "power_efficacy.h"

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

// それぞれのタイマー
int timer_bullet;
int timer_down_bullet;
int timer_shell;
int timer_down_shell;
int timer_ingot;
int timet_down_ingot;
int timer_beam;
int timer_down_beam;
int timer_bankroll;
int timer_down_bankroll;
int timer_special;
int timer_down_special;

// 武器選択値
int serect_weapon;

// カーソルの当たり判定サイズ
float cursor_hitbox_half_width = 10.0f;
float cursor_hitbox_half_height = 10.0f;

// 通常時のスケール
float cursor_normal_scale = CURSOL_MAIN_SCALE;

float cursor_scale = CURSOL_MAIN_SCALE;


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
    
    timer_down_bullet = TIMER;
    timer_bullet = timer_down_bullet;
    timer_down_shell = TIMER_SHELL;
    timer_shell = timer_down_shell;
    timer_ingot = TIMER_INGOT;
    timet_down_ingot = timer_ingot;
    timer_down_beam = TIMER_BEAM;
    timer_beam = timer_down_beam;
    timer_down_bankroll = TIMER_BANKROLL;
    timer_bankroll = timer_down_bankroll;
    timer_down_special = TIMER_SPECIAL;
    timer_special =timer_down_special;

    serect_weapon = 6;

    cursor_main = {};

    cursor_normal_scale = CURSOL_MAIN_SCALE;
    cursor_scale = CURSOL_MAIN_SCALE;

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
        cursor_main.color   = {0.0f, 0.05f, 0.05f, 1.0f};
        cursor_main_state++;

        // わざとbreakしない

    case 2:
    {
        // マウスの位置を取得
        cursor_main_posX = (float)getCursorPosX();
        cursor_main_posY = (float)getCursorPosY();
       
        // クールタイムの上限
        if (timer_bullet > timer_down_bullet) { timer_bullet = timer_down_bullet; }
        if (timer_shell > timer_down_shell) { timer_shell = timer_down_shell; }
        if (timer_ingot > timet_down_ingot) { timer_ingot = timet_down_ingot; }
        if (timer_beam > timer_down_beam) { timer_beam = timer_down_beam; }
        if (timer_bankroll > timer_down_bankroll) { timer_bankroll = timer_down_bankroll; }       
        if (timer_special > timer_down_special) { timer_special = timer_down_special; }
        
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
                //==========================================
                // 一円玉マシンガン
                //==========================================
                if (level_up_weapon[Y1] > 0)
                {
                    if (is_end_firing_rapid())
                    {
                        if (timer_bullet == timer_down_bullet)
                        {
                            shot_bullet();
                            timer_bullet--;
                        }
                        else if (timer_bullet == 0)
                        {
                            timer_bullet = timer_down_bullet;
                        }
                        else
                        {
                            timer_bullet--;
                        }
                    }
                }

                //==========================================
                // 貝殻ミサイル
                //==========================================
                if (level_up_weapon[SHELL] > 0)
                {
                    if (timer_shell == timer_down_shell)
                    {
                        shot_bullet_Shell();
                        timer_shell--;
                    }
                    else if (timer_shell == 0)
                    {
                        timer_shell = timer_down_shell;
                    }
                    else
                    {
                        timer_shell--;
                    }
                }

                //==========================================
                // インゴット
                //==========================================
                if (level_up_weapon[INGOT] > 0)
                {
                    if (timer_ingot == timet_down_ingot)
                    {
                        shot_bullet_Ingot();
                        timer_ingot--;
                    }
                    else if (timer_ingot == 0)
                    {
                        timer_ingot = timet_down_ingot;
                    }
                    else
                    {
                        timer_ingot--;
                    }
                }

                //==========================================
                // ビーム
                //==========================================
                if (level_up_weapon[BEAM] > 0)
                {
                    if (timer_beam == timer_down_beam)
                    {
                        shot_bullet_Beam();
                        timer_beam--;
                    }
                    else if (timer_beam == 0)
                    {
                        timer_beam = timer_down_beam;
                    }
                    else
                    {
                        timer_beam--;
                    }
                }

                //==========================================
                // バンクロール
                //==========================================
                if (level_up_weapon[BANKROLL] > 0)
                {
                    if (timer_bankroll == timer_down_bankroll)
                    {
                        shot_bullet_Bankroll();
                        timer_bankroll--;
                    }
                    else if (timer_bankroll == 0)
                    {
                        timer_bankroll = timer_down_bankroll;
                    }
                    else
                    {
                        timer_bankroll--;
                    }
                }

                //==========================================
                // スペシャル
                //==========================================
               /* if (level_up_weapon[WEAPON_MAX] > 0)
                {
                    if (timer_special == timer_down_special)
                    {
                        shot_bullet_Special();
                        timer_special--;
                    }
                    else if (timer_special == 0)
                    {
                        timer_special = timer_down_special;
                    }
                    else
                    {
                        timer_special--;
                    }
                }*/
                break;
            }
        }
      
        

        // 取得した位置に画像を配置
        cursor_main.pos = {
            cursor_main_posX,
            cursor_main_posY
        };

        cursor_main.scale = {
            cursor_scale,
            cursor_scale
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
    if (cursor_level < level_8)
    {
        cursor_level++;
    }

    // カーソルサイズ
    cursor_main.scale.x += 0.1f;
    cursor_main.scale.y += 0.1f;

    // 当たり判定
    cursor_hitbox_half_width += 2.0f;
    cursor_hitbox_half_height += 2.0f;
}
