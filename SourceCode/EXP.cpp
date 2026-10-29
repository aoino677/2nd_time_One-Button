#include "all.h"
#include "EXP.h"
#include "enemy.h"
#include "player.h"

int EXP_timer = 0;
int EXP_state = 0;

Sprite* sprEXP;
OBJ2D EXP[EXP_COUNT];

void EXP_init()
{
	EXP_timer = 0;
	EXP_state = 0;
}

void EXP_update()
{
	switch (EXP_state)
	{
	case 0:
		//////// 初期設定 ////////

		// EXPの読み込み
		sprEXP = sprite_load(L"./Data/Images/EXP.png");

		EXP_state++;
		break;
	case 1:
        // パラメーターの設定
        for (int i = 0; i < EXP_COUNT; i++)
        {
            EXP[i] = {};
            EXP[i].pos = { enemy->pos.x, enemy->pos.y };
            EXP[i].scale = { EXP_SCALE    , EXP_SCALE };
            EXP[i].texSize = { EXP_TEX_W    , EXP_TEX_H };
            EXP[i].pivot = { EXP_PIVOT_X , EXP_PIVOT_Y };
            EXP[i].color = { 1, 1, 1, EXP[i].color.w};

            EXP[i].timer = 0;                // 経験値が落ちて動くまでの時間
            EXP[i].moveSpeed = 1.0f;     // 落ちたXPの吸収される速度
        }
		EXP_state++;
		break;
	case 2:
        for (int i = 0; i < EXP_COUNT; i++)
        {
            // 敵の色が０（敵が死んだら）になったらEXPが落ちる
            if (enemy->color.w == 0.0f)
            {
                EXP[i].color.w = 1.0f;
                EXP[i].timer++;
                if (EXP[i].timer == 60) // １秒たったらプレイヤーに近づく
                {
                    move_block_player(&EXP[i], &player, EXP[i].moveSpeed);
                }
            }
            else if (enemy->color.w == 1.0f)
            {
                EXP[i].color.w = 0.0f;
            }
        }
		EXP_state++;
		break;
	}

}

void EXP_render()
{
    for (int i = 0; i < EXP_COUNT; i++)
    {
        sprite_render(
            sprEXP,
            EXP[i].pos.x, EXP[i].pos.y,
            EXP_SCALE, EXP_SCALE,
            0, 0,
            EXP_TEX_W, EXP_TEX_H,
            EXP_PIVOT_X, EXP_PIVOT_Y,
            ToRadian(0),
            1, 1, 1, EXP[i].color.w
        );
    }
}

void EXP_deinit()
{
    for (int i = 0; i < EXP_COUNT; i++)
    {
        safe_delete(sprEXP);
    }
}

//==================================================
// プレイヤーに近づくEXP
//==================================================
bool move_block_player(
    OBJ2D* exp,
    OBJ2D* Eplayer,
    float speed
)
{
    // X方向の距離
    float dx = exp->pos.x - Eplayer->pos.x;
    if (dx < 0.0f){dx = -dx;}

    // Y方向の距離
    float dy = exp->pos.y - Eplayer->pos.y;
    if (dy < 0.0f){dy = -dy;}

    // プレイヤーとブロックが重なる範囲
    float limitX =(static_cast<float>(EXP_TEX_W) +static_cast<float>(128)) / 2.0f;
    float limitY =(static_cast<float>(EXP_TEX_H)  +static_cast<float>(128)) / 2.0f;

    // プレイヤーに当たった
    if (dx < limitX && dy < limitY){return true;}

    //==================================================
    // X方向に近づく
    //==================================================
    if (exp->pos.x < Eplayer->pos.x)
    {
        exp->pos.x += speed;
        // プレイヤーを飛び越えないようにする
        if (exp->pos.x > Eplayer->pos.x)
        {
            exp->pos.x = Eplayer->pos.x;
        }
    }
    else if (exp->pos.x > Eplayer->pos.x)
    {
        exp->pos.x -= speed;
        // プレイヤーを飛び越えないようにする
        if (exp->pos.x < Eplayer->pos.x)
        {
            exp->pos.x = Eplayer->pos.x;
        }
    }
    //==================================================
    // Y方向に近づく
    //==================================================
    if (exp->pos.y < Eplayer->pos.y)
    {
        exp->pos.y += speed;
        // プレイヤーを飛び越えないようにする
        if (exp->pos.y > Eplayer->pos.y)
        {
            exp->pos.y = Eplayer->pos.y;
        }
    }
    else if (exp->pos.y > Eplayer->pos.y)
    {
        exp->pos.y -= speed;
        // プレイヤーを飛び越えないようにする
        if (exp->pos.y < Eplayer->pos.y)
        {
            exp->pos.y = Eplayer->pos.y;
        }
    }
    return false;
}
