#include "item.h"
#include "enemy.h"
#include "player.h"
#include "Collision2D.h"
#include "EXP.h"
#include "power_efficacy.h"

OBJ2D item[ITEM_MAX];

// 画像は4種類だけ
Sprite* spritem[4];

// アイテムごとの種類
int item_type[ITEM_MAX];

// アイテムごとの取得ゲージ
int item_timer[ITEM_MAX];

int item_state = 0;
int random;
int lucky;

bool fever_blue;
bool fever_red;
bool fever_pink;

float posX;
float posY;
float LengthX;
float LengthY;

VECTOR2 playerLeftUp;
VECTOR2 playerLength;

float color_w = 0;
bool item_check;

// レッドアイテムの画像
OBJ2D redItem_effect;
Sprite* sprRedItem_effect;
int red_direction;
float red_base_x;

// 初期化
void item_init()
{
    item_state = 0;

    red_direction = 1;
    red_base_x = 0;

    for (int i = 0; i < ITEM_MAX; i++)
    {
        item[i] = {};
        item[i].color.w = 0.0f;

        item_type[i] = -1;
        item_timer[i] = 1;
    }

    fever_blue     = false ;
    fever_red      = false ;
    fever_pink      = false;

    random = 0;
    lucky = ITEM_RANDOM;
    item_check = false;

    posX = 0;
    posY = 0;
    LengthX = 0;
    LengthY = 0;

    music::load(ITEM_BLUE_SE1, L"Data/Sounds/SE/slow_start.wav");
    music::load(ITEM_BLUE_SE2, L"Data/Sounds/SE/slow_end.wav");
    music::load(ITEM_PINK_SE, L"Data/Sounds/SE/click4.wav");
    music::load(ITEM_RED_SE, L"Data/Sounds/BGM/game_boss1.wav");
    music::load(ITME_GREEN_SE, L"Data/Sounds/SE/click1.wav");
}


// 敵が死亡したときのアイテムドロップ
void item_drop(int enemy_index)
{
    // 50回に1回の確率
    if (rand() % lucky != 0)
    {
        return;
    }

    // 空いているアイテムを探す
    for (int i = 0; i < ITEM_MAX; i++)
    {
        if (item[i].color.w != 1.0f)
        {
            // 敵の位置にアイテムを出現
            item[i].pos = enemy[enemy_index].pos;
            item[i].color.w = 1.0f;
            item[i].moveSpeed = 0;

            // 取得ゲージをリセット
            item_timer[i] = 1;

            // 敵の種類によってアイテムを決定
            switch (enemy[enemy_index].type)
            {
            case ENEMY_TYPE_BOLL:
                item_type[i] = rand() % 4;
                break;
            case ENEMY_TYPE_BALLOON:
                item_type[i] = rand() % 4;
                break;
            case ENEMY_TYPE_ROCKET:
                item_type[i] = rand() % 4;
                break;
            case ENEMY_TYPE_BOMB:
                item_type[i] = rand() % 4;
                break;
            case ENEMY_TYPE_CRAYON:
                item_type[i] = rand() % 4;
                break;
            default:
                // 対応していない敵はドロップしない
                item[i].color.w = 0.0f;
                item_type[i] = -1;
                break;
            }
            break;
        }
    }
}

void item_update()
{
    switch (item_state)
    {
    case 0:

        // アイテム画像読み込み
        spritem[0] = sprite_load(L"./Data/Images/itemBlue.png");
        spritem[1] = sprite_load(L"./Data/Images/itemGreen.png");
        spritem[2] = sprite_load(L"./Data/Images/itemRed.png");
        spritem[3] = sprite_load(L"./Data/Images/itemPink.png");

        // 炎の画像
        sprRedItem_effect = sprite_load(L"./Data/Images/fire.png");

        item_state++;
        break;
    case 1:

        // アイテムの初期設定
        for (int i = 0; i < ITEM_MAX; i++)
        {
            item[i] = {};
            item[i].pos      = { -500, -500 };
            item[i].scale    = { ITEM_SCALE, ITEM_SCALE };
            item[i].texSize  = { ITEM_TEXSIZE_X,ITEM_TEXSIZE_Y };
            item[i].pivot    = { ITEM_PIVOT_X,ITEM_PIVOT_Y };
            item[i].color    = { 1, 1, 1, 0 };

            item_type[i] = -1;
            item_timer[i] = 1;
        }
        // 炎の初期設定
        redItem_effect = {};
        redItem_effect.pos      = { 0, 0 };
        redItem_effect.scale    = { REDITEM_EFFECT_SCALE, REDITEM_EFFECT_SCALE };
        redItem_effect.texSize  = { REDITEM_EFFECT_TEXSIZE_X,REDITEM_EFFECT_TEXSIZE_Y };
        redItem_effect.pivot    = { REDITEM_EFFECT_PIVOT_X,REDITEM_EFFECT_PIVOT_Y };
        redItem_effect.color    = { 1, 1, 1, 0.6f };
        red_base_x = 0;
        red_direction = 1;

        item_state++;
        break;
    case 2:

        for (int i = 0; i < ITEM_MAX; i++)
        {
            // アイテムが存在しない場合
            if (item[i].color.w != 1.0f)
            {
                continue;
            }

            // アイテムを移動
            item[i].moveSpeed += 100.0f / 60.0f;

            item[i].pos.x -= item[i].moveSpeed;

            // アイテムが停止位置に到達した場合
            if (item[i].pos.x <
                (SCREEN_H - item[i].pos.y) * 0.3584f / 0.9336f)
            {
                item[i].pos.x =
                    (SCREEN_H - item[i].pos.y) * 0.3584f / 0.9336f;

                // アイテムの当たり判定
                posX = item[i].pos.x - (item[i].pivot.x * item[i].scale.x) * acquisition_range;
                posY = item[i].pos.y - (item[i].pivot.y * item[i].scale.y) * acquisition_range;
                LengthX = (item[i].texSize.x * item[i].scale.x) * acquisition_range;
                LengthY = (item[i].texSize.y * item[i].scale.y) * acquisition_range;
                playerLeftUp = player.pos - VECTOR2{ player.pivot.x * player.scale.x,player.pivot.y * player.scale.y };
                playerLength = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
                color_w = 0.3f;
                // プレイヤーとアイテムが接触しているか
                if (Collision2D::rect_vs_rect( { posX, posY }, { LengthX, LengthY },playerLeftUp,playerLength))
                {
                    // このアイテム専用のゲージを増やす
                    if (item_timer[i] < ITEM_TIMER)
                    {
                        item_timer[i]++;
                    }

                    // アイテムのチェック
                    item_check = true;

                    // ゲージが最大になったら取得
                    if (item_timer[i] >= ITEM_TIMER)
                    {
                        switch (item_type[i])
                        {
                        case 0:
                            Fever_Time_Start_Blue();
                            music::play(ITEM_BLUE_SE1, false);
                            fever_blue = true;
                            break;

                        case 1:
                            EXP_levelUp_ALL();
                            music::play(ITME_GREEN_SE, false);
                            break;

                        case 2:
                            Fever_Time_Start_Red();
                            music::play(ITEM_RED_SE, false);
                            fever_red = true;
                            break;

                        case 3:
                            HP();
                            music::play(ITEM_PINK_SE, false);
                            fever_pink = true;
                            break;
                        }

                        // アイテムを消す
                        item[i].color.w = 0.0f;
                        item_type[i] = -1;
                        item_timer[i] = 1;
                        color_w = 0.0f;
                        break;
                    }
                }
                else
                {
                    // 接触していなければゲージを減らす
                    if (item_timer[i] > 1)
                    {
                        item_timer[i]--;
                    }

                    item_check = false;
                }
            }
        }

        // ブルーの効果
        if (fever_blue == true)
        {
            efficacy_timer++;
            if (efficacy_timer >= 230)
            {
                music::stop(ITEM_BLUE_SE1);
                music::play(ITEM_BLUE_SE2, false);
            }
            if (efficacy_timer >= 300)
            {
                Fever_Time_End_Blue();
                music::stop(ITEM_BLUE_SE2);
                fever_blue = false;
                efficacy_timer = 0;
            }
        }
        // レッドの効果
        if (fever_red == true)
        {
            efficacy_timer++;
            if (efficacy_timer >= 300)
            {
                Fever_Time_End_Red();
                music::stop(ITEM_RED_SE);
                fever_red = false;
                efficacy_timer = 0;
            }
        }
        // ピンクの効果
        if (fever_pink == true)
        {
            efficacy_timer++;
            if (efficacy_timer >= 15)
            {
                fever_pink = false;
                efficacy_timer = 0;
            }
        }
        break;
    }
    // 炎を左右に震わせる
    redItem_effect.pos.x += 3.0f * red_direction;

    if (redItem_effect.pos.x >= red_base_x + 10.0f)
    {
        red_direction = -1;
    }

    if (redItem_effect.pos.x <= red_base_x - 10.0f)
    {
        red_direction = 1;
    }
}


// 描画
void item_render()
{
    for (int i = 0; i < ITEM_MAX; i++)
    {
        // アイテムが存在しない場合
        if (item[i].color.w != 1.0f)
        {
            continue;
        }

        // アイテムの種類に合わせた画像を表示
        if (item_type[i] < 0 || item_type[i] > 3)
        {
            continue;
        }

        sprite_render(
            spritem[item_type[i]],
            item[i].pos.x,item[i].pos.y,
            item[i].scale.x,item[i].scale.y,
            0, 0,
            item[i].texSize.x,item[i].texSize.y,
            item[i].pivot.x,item[i].pivot.y,
            ToRadian(0),
            1, 1, 1,item[i].color.w
        );

        // 取得ゲージの背景
        float bar_pos_y = item[i].pos.y - 60.0f;

        //ゲージが最小の時は表示しない
        if (item_timer[i] > 1)
        {
            // 取得ゲージ
            primitive::rect(item[i].pos.x - 60, bar_pos_y, ITEM_TIMER, 10, 0, 0, 0, 0, 0, 0, 0.5f);
            primitive::rect(item[i].pos.x - 60, bar_pos_y, item_timer[i], 10, 0, 0, 0, 0, 0, 0, 1.0f);
        }
    }
    if (fever_red == true)
    {
        sprite_render(
            sprRedItem_effect,
            player.pos.x + redItem_effect.pos.x, player.pos.y,
            redItem_effect.scale.x, redItem_effect.scale.y,
            0, 0,
            redItem_effect.texSize.x, redItem_effect.texSize.y,
            redItem_effect.pivot.x, redItem_effect.pivot.y,
            ToRadian(0),
            redItem_effect.color.x, redItem_effect.color.y, redItem_effect.color.z, redItem_effect.color.w
        );
    }

    if (fever_blue == true)
    {
        primitive::rect(
            0, 0,
            SCREEN_W, SCREEN_H,
            0, 0,
            ToRadian(0),
            0.70f, 0.85f, 1.00f, 0.3f
        );
    }

    if (fever_pink == true)
    {
        primitive::rect(
            0, 0,
            SCREEN_W, SCREEN_H,
            0, 0,
            ToRadian(0),
            1.000f, 0.714f, 0.757f, 0.5f
        );
    }
}


// 終了処理
void item_deinit()
{
    for (int i = 0; i < 4; i++)
    {
        safe_delete(spritem[i]);
    }
    safe_delete(sprRedItem_effect);
    music::clear();
}
