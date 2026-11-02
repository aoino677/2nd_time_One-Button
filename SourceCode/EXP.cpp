#include "all.h"
#include "EXP.h"
#include "enemy.h"
#include "player.h"
#include <vector>

int EXP_timer = 0;
int EXP_state = 0;

Sprite* sprEXP;
OBJ2D EXP[EXP_COUNT];
//std::vector<OBJ2D> EXP;

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
            EXP[i].pos = { enemy[i].pos.x, enemy[i].pos.y};
            EXP[i].scale = { EXP_SCALE    , EXP_SCALE };
            EXP[i].texSize = { EXP_TEX_W    , EXP_TEX_H };
            EXP[i].pivot = { EXP_PIVOT_X , EXP_PIVOT_Y };
            EXP[i].color = { 1, 1, 1, 0};

            EXP[i].timer = 0;                // 経験値が落ちて動くまでの時間
            EXP[i].moveSpeed = 0.0f;     // 落ちたXPの吸収される速度
        }
		EXP_state++;
		break;
	case 2:
        for (int i = 0; i < enemy_max; i++)
        {
            // 敵の色が０（敵が死んだら）になったらEXPが落ちる
            if (enemy[i].hp == 0)
            {
                for (int j = 0; j < EXP_COUNT; j++)
                {
                    if (EXP[j].color.w != 1.0f)
                    {
                        EXP[j].pos = enemy[i].pos;
                        EXP[j].color.w = 1.0f;
                        EXP[j].moveSpeed = 0;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < EXP_COUNT; j++)
        {
            EXP[j].moveSpeed += 1.5f / 60.0f;
            EXP[j].pos.y += EXP[j].moveSpeed;
            if (EXP[j].pos.y > 1000) { EXP[j].color.w = 0.0f;/*EXP[j].pos.y = 1000;*/ }
        }

		//EXP_state++;
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
            EXP[i].scale.x, EXP[i].scale.y,
            0, 0,
            EXP[i].texSize.x, EXP[i].texSize.y,
            EXP[i].pivot.x, EXP[i].pivot.y,
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
