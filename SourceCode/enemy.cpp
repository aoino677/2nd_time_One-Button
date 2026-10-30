#include "all.h"
#include "player.h"
#include "enemy.h"
#include "OBJ2D.h"
#include <vector>
#include <cstdlib>

Sprite* sprEnemy;         
OBJ2D enemy[ENEMY_NUMBER];
int enemy_state;
int enemy_timer;
int enemy_turbo[ENEMY_NUMBER];
int enemy_number = 0;
float enemy_speed = 3.0f; // 敵の移動速度
float angle[ENEMY_NUMBER];
int enemy_max = 3;


void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
	enemy_turbo[50] = 0;
}

void enemy_deinit()
{
	safe_delete(sprEnemy);
}

void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		// スプライトの読み込み
		sprEnemy = sprite_load(L"Data/Images/enemy.png");
		
		enemy_state++;
		break;

	case 1:
		//パラメーターの設定と初期出現
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			enemy[i].scale = { ENEMY_SCALE , ENEMY_SCALE };
			enemy[i].texSize = { ENEMY_TEX_W , ENEMY_TEX_H };
			enemy[i].pivot = { ENEMY_PIVOT_X , ENEMY_PIVOT_Y };
			enemy[i].color = { 1, 1, 1, 1 };

			//初期位置にランダムにスポーン
			enemy_respawn(i);
		}
		
		enemy_state++;
		break;

	case 2:
		for (int i = 0; i < enemy_max; i++)
		{
			//HPが0以下になった時に別の敵として再出現させる
			if (enemy[i].hp <= 0)
			{
				enemy_respawn(i);
			}
            enemy_move(i);
			enemy_turbo[i]++;
		}

		if (enemy_timer > 600)
		{
			if (enemy_max < ENEMY_NUMBER) 
			{
				enemy_max++;
				enemy_speed += 0.5;
			}
			enemy_timer = 0;
		}

		break;
	}

	enemy_timer++;
	
}

void enemy_move(int i)
{
	// 中心に向かう基本角度を計算
	float base_angle = atan2f(540.0f - enemy[i].pos.y, 960.0f - enemy[i].pos.x);

	// 敵の種類ごとの動きの処理
	if (enemy[i].type == ENEMY_TYPE_NORMAL)
	{
		// --- 通常の敵：まっすぐ進む ---
		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;
	}
	else if (enemy[i].type == ENEMY_TYPE_TRICKY)
	{
		//トリッキーな敵：うねうね揺れながら進む 
		enemy[i].move_timer++; // 個別タイマーを進める

		//サイン波を使って左右に揺れる幅を計算
		float wobble = sinf(enemy[i].move_timer * 0.15f) * 15.0f;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * wobble;
		enemy[i].pos.y += cosf(base_angle) * wobble;
	}
	else if (enemy[i].type == ENEMY_TYPE_TURBO)
	{
		if (enemy_turbo[i] > 60)
		{
			enemy[i].pos.x += cosf(base_angle) * enemy_speed*10 ;
			enemy[i].pos.y += sinf(base_angle) * enemy_speed*10 ;
			enemy_turbo[i] = 0;
		}
		
	}
}



// 敵を画面外のランダムな位置に出現させる
void enemy_respawn(int i)
{
	enemy[i].hp = 1; // HPを復活させる
	enemy[i].move_timer = 0; //タイマーをリセット

	//敵の選別
	if (rand() % 100 < 30)
		enemy[i].type = ENEMY_TYPE_TRICKY;

	else if (rand() % 100 >= 30 && rand() % 100 < 60)
		enemy[i].type = ENEMY_TYPE_TURBO;
	
	else if(rand() % 100 > 60)
	    enemy[i].type = ENEMY_TYPE_NORMAL;
	

	// 画面外のランダムな位置を決める
	int side = rand() % 4;
	switch (side)
	{
	case 0: // 上の外側
		enemy[i].pos.x = rand() % 1920;
		enemy[i].pos.y = -50.0f;
		break;
	case 1: // 下の外側
		enemy[i].pos.x = rand() % 1920;
		enemy[i].pos.y = 1080.0f + 50.0f;
		break;
	case 2: // 左の外側
		enemy[i].pos.x = -50.0f;
		enemy[i].pos.y = rand() % 1080;
		break;
	case 3: // 右の外側
		enemy[i].pos.x = 1920.0f + 50.0f;
		enemy[i].pos.y = rand() % 1080;
		break;
	}
}

void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].hp <= 0) { continue; }

		sprite_render(
			sprEnemy,
			enemy[i].pos.x, enemy[i].pos.y,
			enemy[i].scale.x, enemy[i].scale.y,
			0, 0,
			enemy[i].texSize.x, enemy[i].texSize.y,
			enemy[i].pivot.x, enemy[i].pivot.y,
			ToRadian(0),
			enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w
		);
	}
}