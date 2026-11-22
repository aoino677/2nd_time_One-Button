#include "EXP.h"
#include "item.h"
#include "enemy.h"
#include "player.h"
#include <vector>
#include "Collision2D.h"
#include "power_efficacy.h"
#include "effect_burst.h"

int EXP_timer = 0;
int EXP_state = 0;
int exp_count;
float acquisition_range;

#define EXP_SPR_COUNT 4
Sprite* sprEXP[EXP_SPR_COUNT];
OBJ2D EXP[EXP_COUNT];
//std::vector<OBJ2D> EXP;

float posX_EXP;
float posY_EXP;
float LengthX_EXP;
float LengthY_EXP;
VECTOR2 playerLeftUp_EXP;
VECTOR2 playerLength_EXP;
float color_w_EXP = 0;

//前回HPを記憶する配列
static int prev_enemy_hp[ENEMY_NUMBER] = { 0 };

//経験値の種類を表す定数
#define EXP_TYPE_1YEN 0
#define EXP_TYPE_2YEN 1
#define EXP_TYPE_4YEN 2
#define EXP_TYPE_8YEN 3


void EXP_init()
{
	EXP_timer = 0;
	EXP_state = 0;
	exp_count = 0;
	acquisition_range = 1.0f;
}

void EXP_update()
{
#ifdef _DEBUG
	if (TRG(0) & PAD_TRG4)
	{
		EXP_levelUp_ALL();
	}
#endif // _DEBUG

	switch (EXP_state)
	{
	case 0:
		//////// 初期設定 ////////

		// EXPの読み込み
		sprEXP[EXP_TYPE_1YEN] = sprite_load(L"./Data/Images/1yen.png");
		sprEXP[EXP_TYPE_2YEN] = sprite_load(L"./Data/Images/10yen.png");
		sprEXP[EXP_TYPE_4YEN] = sprite_load(L"./Data/Images/100yen.png");
		sprEXP[EXP_TYPE_8YEN] = sprite_load(L"./Data/Images/500yen.png");

		EXP_state++;
		break;
	case 1:
		// パラメーターの設定
		for (int i = 0; i < EXP_COUNT; i++)
		{
			EXP[i] = {};
			EXP[i].pos = { enemy[i].pos.x, enemy[i].pos.y };
			EXP[i].scale = { EXP_SCALE    , EXP_SCALE };
			EXP[i].texSize = { EXP_TEX_W    , EXP_TEX_H };
			EXP[i].pivot = { EXP_PIVOT_X , EXP_PIVOT_Y };
			EXP[i].color = { 1, 1, 1, 0 };
			EXP[i].type = EXP_TYPE_1YEN;
		}
		EXP_state++;
		break;
	case 2:
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{

			//HPが0以下になった瞬間を検知する
			if (prev_enemy_hp[i] > 0 && enemy[i].hp <= 0)
			{
				//空いているEXPを探してドロップさせる
				for (int exp_index = 0; exp_index < EXP_COUNT; exp_index++)
				{
					if (EXP[exp_index].color.w != 1.0f)
					{
						EXP[exp_index].pos = enemy[i].pos;
						EXP[exp_index].color.w = 1.0f;
						EXP[exp_index].moveSpeed = 0;
						float moveY = 150.0f;
						EXP[exp_index].moveSpeedY = (rand() / static_cast<float>(RAND_MAX) * moveY * 2 - moveY) / 60.0f;
						EXP[exp_index].type = EXP_TYPE_1YEN;
						break;
					}
				}
			}

			//次のフレームの比較用に、現在のHPを保存しておく
			prev_enemy_hp[i] = enemy[i].hp;
		}

		//既存のEXPの移動・プレイヤーとの当たり判定
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
				color_w_EXP = 0.5f; // 当たり判定の透明度
				
				{
					EXP[j].timer++;
					if (EXP[j].timer > 3 * system::FRAME_RATE && EXP[j].type < EXP_SPR_COUNT - 1)
					{
						EXP[j].timer = 0;
						EXP[j].type++;

						float range = 5;
						particles_update
						(EXP[j].pos.x, EXP[j].pos.y, range, 0, 0, 0);
					}
				}

				VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
				posX_EXP = EXP[j].pos.x - (EXP[j].pivot.x * EXP[j].scale.x) * acquisition_range;
				posY_EXP = EXP[j].pos.y - (EXP[j].pivot.y * EXP[j].scale.y) * acquisition_range;
				LengthX_EXP = (EXP[j].texSize.x * EXP[j].scale.x) * acquisition_range;
				LengthY_EXP = (EXP[j].texSize.y * EXP[j].scale.y) * acquisition_range;
				playerLeftUp_EXP = player.pos - VECTOR2{ player.pivot.x * player.scale.x, player.pivot.y * player.scale.y };
				playerLength_EXP = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
				if (Collision2D::rect_vs_rect({ posX_EXP, posY_EXP }, { LengthX_EXP, LengthY_EXP }, playerLeftUp_EXP, playerLength_EXP))
				{
					switch (EXP[j].type)
					{
					case EXP_TYPE_1YEN:
						exp_count += COUNTER_1Y;
						break;
					case EXP_TYPE_2YEN:
						exp_count += COUNTER_10Y;
						break;
					case EXP_TYPE_4YEN:
						exp_count += COUNTER_100Y;
						break;
					case EXP_TYPE_8YEN:
						exp_count += COUNTER_500Y;
						break;
					}
					EXP[j].color.w = 0;
					color_w_EXP = 0.0f;
				}
			}
			else
			{
				EXP[j].pos.y += EXP[j].moveSpeedY;
				if (EXP[j].pos.y < 0)EXP[j].pos.y = 0;
				else if (EXP[j].pos.y > SCREEN_H)EXP[j].pos.y = SCREEN_H;
			}
		}
		break;
	}
}

/*case 2:
for (int enemy_index = 0; enemy_index < enemy_max; enemy_index++)
{
	// 敵の色が０（敵が死んだら）になったらEXPが落ちる
	if (enemy[enemy_index].hp <= 0)
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
		color_w_EXP = 0.5f; // 当たり判定の透明度
		VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
		posX_EXP = EXP[j].pos.x - (EXP[j].pivot.x * EXP[j].scale.x) * acquisition_range;
		posY_EXP = EXP[j].pos.y - (EXP[j].pivot.y * EXP[j].scale.y) * acquisition_range;
		LengthX_EXP = (EXP[j].texSize.x * EXP[j].scale.x) * acquisition_range;
		LengthY_EXP = (EXP[j].texSize.y * EXP[j].scale.y) * acquisition_range;
		playerLeftUp_EXP = player.pos - VECTOR2{ player.pivot.x * player.scale.x, player.pivot.y * player.scale.y };
		playerLength_EXP = { player.texSize.x * player.scale.x, player.texSize.y * player.scale.y };
		if (Collision2D::rect_vs_rect({posX_EXP, posY_EXP }, {LengthX_EXP, LengthY_EXP }, playerLeftUp_EXP, playerLength_EXP))
		{
			exp_count++;
			EXP[j].color.w = 0;
			color_w_EXP = 0.0f;
		}
	}
}
break;
}
}*/

void EXP_render()
{
	for (int i = 0; i < EXP_COUNT; i++)
	{
		sprite_render(
			sprEXP[EXP[i].type],
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
	for (int i = 0; i < EXP_SPR_COUNT; i++)
	{
		safe_delete(sprEXP[i]);
	}
}

void EXP_levelUp_ALL()
{
	for (int i = 0; i < EXP_COUNT; i++)
	{
		float range = 15;
		if (EXP[i].color.w != 1.0f)continue;
		if (EXP[i].type < EXP_SPR_COUNT - 1)
		{
			EXP[i].type++;
			particles_update
			(EXP[i].pos.x, EXP[i].pos.y, range, 0, 0, 0);
		};
	}

}
