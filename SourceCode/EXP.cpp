#include "EXP.h"
#include "enemy.h"
#include "player.h"
#include <vector>
#include "Collision2D.h"

int EXP_timer = 0;
int EXP_state = 0;
int exp_count;

Sprite* sprEXP;
OBJ2D EXP[EXP_COUNT];
//std::vector<OBJ2D> EXP;

void EXP_init()
{
	EXP_timer = 0;
	EXP_state = 0;
    exp_count = 0;
}

void EXP_update()
{
	switch (EXP_state)
	{
	case 0:
		//////// 初期設定 ////////

		// EXPの読み込み
		sprEXP = sprite_load(L"./Data/Images/1yen.png");

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
        }
		EXP_state++;
		break;
	case 2:
        for (int enemy_index = 0; enemy_index < enemy_max; enemy_index++)
        {
            // 敵の色が０（敵が死んだら）になったらEXPが落ちる
            if (enemy[enemy_index].hp == 0)
            {
                for (int exp_index = 0; exp_index < EXP_COUNT; exp_index++)
                {
                    if (EXP[exp_index].color.w != 1.0f)
                    {
                        EXP[exp_index].pos = enemy[enemy_index].pos;
                        EXP[exp_index].color.w = 1.0f;
                        EXP[exp_index].moveSpeed = 0;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < EXP_COUNT; j++)
        {
            if (EXP[j].color.w != 1.0f)
            {
                EXP[j].pos = { -500, -500 };
                continue;
            }
            EXP[j].moveSpeed += 100.0f / 60.0f;
            EXP[j].pos.x -= EXP[j].moveSpeed;
            if (EXP[j].pos.x < (SCREEN_H - EXP[j].pos.y) * 0.3584f / 0.9336f)
            {
                EXP[j].pos.x = (SCREEN_H - EXP[j].pos.y) * 0.3584f / 0.9336f;
                VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
                float posX = EXP[j].pos.x - EXP[j].pivot.x * EXP[j].scale.x;
                float posY = EXP[j].pos.y - EXP[j].pivot.y * EXP[j].scale.y;
                float LengthX = EXP[j].texSize.x * EXP[j].scale.x;
                float LengthY = EXP[j].texSize.y * EXP[j].scale.y;
                VECTOR2 playerLeftUp = player.pos - VECTOR2{ player.pivot.x * player.scale.x, player.pivot.y * player.scale.y };
                VECTOR2 playerLength = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
                if (Collision2D::rect_vs_rect({posX, posY}, {LengthX, LengthY}, playerLeftUp, playerLength))
                {
                    exp_count++;
                    EXP[j].color.w = 0;
                }
            }
        }
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
    {
        safe_delete(sprEXP);
    }
}
