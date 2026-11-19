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

int Blue_counter;
int Green_counter;
int Red_counter;
int Pink_counter;

bool fever_blue;
bool fever_red;

float posX;
float posY;
float LengthX;
float LengthY;

VECTOR2 playerLeftUp;
VECTOR2 playerLength;

float color_w = 0;


// 初期化
void item_init()
{
    item_state = 0;

    Blue_counter = 0;
    Green_counter = 0;
    Red_counter = 0;
    Pink_counter = 0;

    for (int i = 0; i < ITEM_MAX; i++)
    {
        item[i] = {};
        item[i].color.w = 0.0f;

        item_type[i] = -1;
        item_timer[i] = 1;
    }

    fever_blue     = false ;
    fever_red      = false ;

    random = 0;
    lucky = ITEM_RANDOM;

    posX = 0;
    posY = 0;
    LengthX = 0;
    LengthY = 0;
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
            case ENEMY_TYPE_BOBM:
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

        // 画像読み込み
        spritem[0] = sprite_load(L"./Data/Images/itemBlue.png");
        spritem[1] = sprite_load(L"./Data/Images/itemGreen.png");
        spritem[2] = sprite_load(L"./Data/Images/itemRed.png");
        spritem[3] = sprite_load(L"./Data/Images/itemPink.png");

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

                    // ゲージが最大になったら取得
                    if (item_timer[i] >= ITEM_TIMER)
                    {
                        switch (item_type[i])
                        {
                        case 0:
                            Fever_Time_Start_Blue();
                            fever_blue = true;
                            break;

                        case 1:
                            EXP_levelUp_ALL();
                            break;

                        case 2:
                            Fever_Time_Start_Red();
                            fever_red = true;
                            break;

                        case 3:
                            HP();
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
                }
            }
        }

        // ブルーの効果
        if (fever_blue == true)
        {
            efficacy_timer++;
            if (efficacy_timer >= 300)
            {
                Fever_Time_End_Blue();
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
                fever_red = false;
                efficacy_timer = 0;
            }
        }
        break;
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

        primitive::rect(item[i].pos.x - 60, bar_pos_y, ITEM_TIMER, 10, 0, 0, 0, 0, 0, 0, 0.5f);

        // 取得ゲージ
        primitive::rect(item[i].pos.x - 60, bar_pos_y, item_timer[i], 10, 0, 0, 0, 0, 0, 0, 1.0f);
    }
}


// 終了処理
void item_deinit()
{
    for (int i = 0; i < 4; i++)
    {
        safe_delete(spritem[i]);
    }
}