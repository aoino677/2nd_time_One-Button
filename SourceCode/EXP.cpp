#include "all.h"
#include "EXP.h"
#include "enemy.h"
#include "player.h"

int EXP_timer = 0;
int EXP_state = 0;

Sprite* sprEXP;
OBJ2D EXP;

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
		EXP = {};
		//EXP.pos = { enemy.pos.x, enemy.pos.y };
		EXP.scale    = { EXP_SCALE    , EXP_SCALE };
		EXP.texSize = { EXP_TEX_W    , EXP_TEX_H };
		EXP.pivot    = { EXP_PIVOT_X , EXP_PIVOT_Y };
		EXP.color   = { 1, 1, 1, EXP.color.w };

		EXP.timer   = 0;
		EXP.moveSpeed = 1.0f;

		EXP_state++;
		break;
	case 2:
		// if (enemy.color.w == 0.0f )
		// {
		//		EXP.color.w = 1.0f;
		//		EXP.timer++;
		//		if (EXP.timer == 60 )
		//		{
		//			move_block_player( &EXP, &Player, EXP.moveSpeed );
		//		}
		// }else if (enemy.color.w == 1.0f )
		// {
		//		EXP.color.w = 0.0f;
		// }

		EXP_state++;
		break;
	}

}

void EXP_render()
{
	sprite_render(
		sprEXP,
		EXP.pos.x, EXP.pos.y,
        EXP_SCALE, EXP_SCALE,
		0, 0,
        EXP_TEX_W, EXP_TEX_H,
        EXP_PIVOT_X, EXP_PIVOT_Y,
		ToRadian(0),
		1, 1, 1, EXP.color.w
	);
}

void EXP_deinit()
{
	safe_delete(sprEXP);
}

//==================================================
// プレイヤーに近づくEXP
//==================================================
//bool move_block_player(
//    OBJ2D* EXP,
//    OBJ2D* Player,
//    float speed
//)
//{
//    // X方向の距離
//    float dx = EXP->pos.x - Player->pos.x;
//    if (dx < 0.0f){dx = -dx;}
//
//    // Y方向の距離
//    float dy = EXP->pos.y - Player->pos.y;
//    if (dy < 0.0f){dy = -dy;}
//
//    // プレイヤーとブロックが重なる範囲
//    float limitX =(static_cast<float>(EXP_TEX_W) +static_cast<float>(PLAYER_TEX_W)) / 2.0f;
//    float limitY =(static_cast<float>(EXP_TEX_H)  +static_cast<float>(PLAYER_TEX_H)) / 2.0f;
//
//    // プレイヤーに当たった
//    if (dx < limitX && dy < limitY){return true;}
//
//    //==================================================
//    // X方向に近づく
//    //==================================================
//    if (EXP->pos.x < Player->pos.x)
//    {
//        EXP->pos.x += speed;
//        // プレイヤーを飛び越えないようにする
//        if (EXP->pos.x > Player->pos.x)
//        {
//            EXP->pos.x = Player->pos.x;
//        }
//    }
//    else if (EXP->pos.x > Player->pos.x)
//    {
//        EXP->pos.x -= speed;
//        // プレイヤーを飛び越えないようにする
//        if (EXP->pos.x < Player->pos.x)
//        {
//            EXP->pos.x = Player->pos.x;
//        }
//    }
//    //==================================================
//    // Y方向に近づく
//    //==================================================
//    if (EXP->pos.y < Player->pos.y)
//    {
//        EXP->pos.y += speed;
//        // プレイヤーを飛び越えないようにする
//        if (EXP->pos.y > Player->pos.y)
//        {
//            EXP->pos.y = Player->pos.y;
//        }
//    }
//    else if (EXP->pos.y > Player->pos.y)
//    {
//        EXP->pos.y -= speed;
//        // プレイヤーを飛び越えないようにする
//        if (EXP->pos.y < Player->pos.y)
//        {
//            EXP->pos.y = Player->pos.y;
//        }
//    }
//    return false;
//}
