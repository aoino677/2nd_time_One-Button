#include "all.h"
#include "player.h"
#include "enemy.h"
#include "OBJ2D.h"
#include "shake.h"
#include <vector>
#include <cstdlib>
#include <math.h>

Sprite* sprEnemy;
OBJ2D enemy[ENEMY_NUMBER];
int enemy_state;
int enemy_timer;
int enemy_boss_timer;
int enemy_max;
int enemy_turbo[ENEMY_NUMBER];
float angle[ENEMY_NUMBER];
float base_angle[ENEMY_NUMBER];
float enemy_speed;
extern int player_hp;
bool boss_respawn;


//エネミー初期化
void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
	enemy_boss_timer = 0;
	enemy_max = 3;
	enemy_speed = 1.0f;
	boss_respawn = false;
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

				if (boss_respawn == true && enemy_boss_timer > 30)
				{
					enemy_boss_timer = 0;
					boss_respawn = false;
				}
			}
			
			//左端にもどっだら右にリスポーン
			if (enemy[i].pos.x < -128)
			{
				enemy[i].pos.x = 1920.0f + 200.0f;
				enemy[i].pos.y = rand() % 756;
				player_hp--;
				shake(10, 20.0f);
			}
			
			enemy_move(i);
			enemy_turbo[i]++;
		}
		
		
		//敵とスピードの増加
		if (enemy_timer> 600)
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

	enemy_boss_timer++;
	enemy_timer++;

}


//敵の種類ごとの動きの処理
void enemy_move(int i)
{
	//プレイヤーに向かう基本角度を計算
	base_angle[i] = atan2f(rand() % 264 + 192 - enemy[i].pos.y, -200.0f - enemy[i].pos.x);

	//通常の敵
	if (enemy[i].type == ENEMY_TYPE_NORMAL)
	{
		enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed;
	}
	//速い敵
	if (enemy[i].type == ENEMY_TYPE_FAST)
	{
		enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed * 2;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed * 2;
	}
	//一定の間隔で進む敵
	else if (enemy[i].type == ENEMY_TYPE_TURBO)
	{
		if (enemy_turbo[i] < 15)
		{
			enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed * 5;
			enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed * 5;
		}

		else if (enemy_turbo[i] > 100)
		{
			enemy_turbo[i] = 0;
		}
	}
	//揺れ動く敵
	else if (enemy[i].type == ENEMY_TYPE_TRICKY)
	{
		//トリッキーな敵：うねうね揺れながら進む 
		enemy[i].move_timer++; // 個別タイマーを進める

		//サイン波を使って左右に揺れる幅を計算
		float wobble = sinf(enemy[i].move_timer * 0.15f) * 20.0f;

		enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed;

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle[i]) * wobble;
		enemy[i].pos.y += cosf(base_angle[i]) * wobble;
	}
	//ボム敵
	else if (enemy[i].type == ENEMY_TYPE_BOBM)
	{
		enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed;

		//爆発したら周囲の敵にダメージ
		if (enemy[i].hp <= 0)
		{
			for (int j = 0; j < ENEMY_NUMBER; j++)
			{
				float enemy_Vec_x = enemy[j].pos.x - enemy[i].pos.x;
				float enemy_Vec_y = enemy[j].pos.y - enemy[i].pos.y;
				
				//大きさが負なら正に戻す
				if (enemy_Vec_x == -enemy_Vec_x)
					enemy_Vec_x *= -1;
				if (enemy_Vec_y == -enemy_Vec_x)
					enemy_Vec_y *= -1;
				
				//ベクトルに大きさを計算
				float enemy_Vec = sqrt(enemy_Vec_x * enemy_Vec_x + enemy_Vec_y * enemy_Vec_y);
				
				if (enemy_Vec>0&&enemy_Vec < 100)
				{
					enemy[j].hp--;
				}

			}
		}
	}
	
	
	
	//ボス敵
	else if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		if (enemy[i].pos.x > 1200)
		{
			enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed * 2;
			enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed * 0.5f;
		}
		else
		{
			enemy[i].move_timer++;
			float wobble = sinf(enemy[i].move_timer * 0.15f) * 3.0f;
			enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed;
			enemy[i].pos.y += cosf(base_angle[i]) * wobble;
		}
		
	}

	
	
	
	
	
	
	//渦巻く敵
	/*else if (enemy[i].type == ENEMY_TYPE_EDDY)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * enemy_speed;
		enemy[i].pos.y += cosf(base_angle) * enemy_speed;
	}*/
}

void enemy_boss_move(int i)
{
	if (enemy[i].pos.x > 1200)
	{
		enemy[i].pos.x += cosf(base_angle[i]) * enemy_speed * 2;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed * 0.5f;
	}
	else
	{
		enemy[i].move_timer++;
		float wobble = sinf(enemy[i].move_timer * 0.15f) * 3.0f;
		enemy[i].pos.y += sinf(base_angle[i]) * enemy_speed;
		enemy[i].pos.y += cosf(base_angle[i]) * wobble;
	}


}


//敵を画面外のランダムな位置に出現させる
void enemy_respawn(int i)
{
	enemy[i].move_timer = 0; //タイマーをリセット

	//敵の選別
	if (enemy_boss_timer < 3000)
	{
		enemy[i].hp = 1; // HPを復活させる
		
		int enemy_random = rand() % 100;

		/*if (enemy_random < 25)
			enemy[i].type = ENEMY_TYPE_NORMAL;

		else if (enemy_random >= 25 && enemy_random < 50)
			enemy[i].type = ENEMY_TYPE_FAST;

		else if (enemy_random >= 50 && enemy_random < 75)
			enemy[i].type = ENEMY_TYPE_TURBO;

		else if (enemy_random >= 75&& enemy_random<90)
			enemy[i].type = ENEMY_TYPE_TRICKY;*/
		
		
		if (enemy_random < 25)
			enemy[i].type = ENEMY_TYPE_NORMAL;
		else if(enemy_random >=25)
			enemy[i].type = ENEMY_TYPE_BOBM;
	}
	else
	{
		enemy[i].type = ENEMY_TYPE_BOSS;
		enemy[i].hp = 20;
		boss_respawn = true;

	}
	
	//生成場所
	if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		enemy[i].pos.x = 1920.0f +200.0f;
		enemy[i].pos.y = rand() % 264 + 192;
	}
	else
	{
		enemy[i].pos.x = 1920.0f + 50.0f;
		enemy[i].pos.y = rand() % 648;
	}
	
	
}


void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].hp <= 0) { continue; }

		if (enemy[i].type == ENEMY_TYPE_BOSS)
		{
			sprite_render(
				sprEnemy,
				enemy[i].pos.x, enemy[i].pos.y,
				3, 3,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w
			);
		}
		else if(enemy[i].type == ENEMY_TYPE_BOBM)
		{
			sprite_render(
				sprEnemy,
				enemy[i].pos.x, enemy[i].pos.y,
				enemy[i].scale.x, enemy[i].scale.y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, 0.8
			);
		}
		else
		{
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

	debug::setString("enemy_boss_timer:%d", enemy_boss_timer);
}