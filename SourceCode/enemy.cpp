#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include <vector>
#include <cstdlib>

Sprite* sprEnemy;
OBJ2D enemy[ENEMY_NUMBER];
int enemy_state;
int enemy_timer;
int enemy_max;
int enemy_hp;
int enemy_boss_timer;
int enemy_turbo[ENEMY_NUMBER];
float angle[ENEMY_NUMBER];
float enemy_speed; 

//エネミー初期化
void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
	enemy_boss_timer = 0;
	enemy_max = 3;
	enemy_speed = 3.0f;
	enemy_hp = 1;
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
			//エネミーの動き
			enemy_ability(i);
			
			
			//HPが0になった時に別の敵として再出現させる
			if (enemy[i].hp <= 0)
			{
				enemy_respawn(i);

				if (enemy[i].type == ENEMY_TYPE_BOSS)
				     enemy_boss_timer = 0;
			}
		    
			
			if (enemy[i].pos.x < -128)
			{
				enemy[i].pos.x = 1920.0f + 50.0f;
				enemy[i].pos.y = rand() % 756;
				player_hp--;
				shake(10, 20.0f);
			}
			
			enemy_turbo[i]++;
		}
		
		
		//秒数がたったら強化
		if (enemy_timer> 6000)
		{
			enemy_max++;
			enemy_hp++;
			enemy_speed += 0.5;
			enemy_timer = 0;
		}

		break;
	}

	enemy_timer++;
	enemy_boss_timer++;

}


//敵の種類ごとの能力
void enemy_ability(int i)
{
	//中心に向かう基本角度を計算
	srand((unsigned int)time(NULL));
	
	float base_angle = atan2f(540 - enemy[i].pos.y, -200.0f - enemy[i].pos.x);

	//通常の敵
	if (enemy[i].type == ENEMY_TYPE_NORMAL)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;
	}
	
	//速い敵
	if (enemy[i].type == ENEMY_TYPE_FAST)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed * 2;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed * 2;
	}
	
	//一定の間隔で進む敵
	else if (enemy[i].type == ENEMY_TYPE_TURBO)
	{
		if (enemy_turbo[i] < 15)
		{
			enemy[i].pos.x += cosf(base_angle) * enemy_speed * 5;
			enemy[i].pos.y += sinf(base_angle) * enemy_speed * 5;
		}
        else if (enemy_turbo[i] > 100)
		    enemy_turbo[i] = 0;
	}
	
	//揺れ動く敵
	else if (enemy[i].type == ENEMY_TYPE_TRICKY)
	{
		//トリッキーな敵：うねうね揺れながら進む 
		enemy[i].move_timer++; 

		//サイン波を使って左右に揺れる幅を計算
		float wobble = sinf(enemy[i].move_timer * 0.15f) * 20.0f;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * wobble;
		enemy[i].pos.y += cosf(base_angle) * wobble;
	}
	
	//渦巻く敵
	else if (enemy[i].type == ENEMY_TYPE_EDDY)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * enemy_speed;
		enemy[i].pos.y += cosf(base_angle) * enemy_speed;
	}
	
	//ボム敵
	else if (enemy[i].type == ENEMY_TYPE_BOBM)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed;

		if (enemy[i].hp <= 0)
		{
			for (int j = 0; j < ENEMY_NUMBER; j++)
			{
				float enemy_Vec_x[ENEMY_NUMBER];
				enemy_Vec_x[j] = enemy[j].pos.x - enemy[i].pos.x;
				
				float enemy_Vec_y[ENEMY_NUMBER];
				enemy_Vec_y[j] = enemy[j].pos.y - enemy[i].pos.y;
				
				//大きさが負なら正に戻す
				if (enemy_Vec_x[j] < 0)
					enemy_Vec_x[j] *= -1;
				if (enemy_Vec_y[j] < 0)
					enemy_Vec_y[j] *= -1;

				float enemy_Vec[ENEMY_NUMBER];
				enemy_Vec[j]= sqrt(enemy_Vec_x[j] * enemy_Vec_x[j] + enemy_Vec_y[j] + enemy_Vec_y[j]);

				if (enemy_Vec[j] < 10)
					enemy[j].hp--;
			}
		}
	}
	
	//ボス敵
	else if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		enemy[i].pos.x += cosf(base_angle) * enemy_speed * 0.5f;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed * 0.5f;
	}
}


//敵を画面外のランダムな位置に出現させる
void enemy_respawn(int i)
{
	enemy[i].move_timer = 0; //タイマーをリセット

	enemy[i].scale = { 1, 1 };

	//敵の選別
	if (enemy_boss_timer < 3000)
	{
		enemy[i].hp =enemy_hp;
		int enemy_random = rand() % 100;
		
		//敵の選別
		if (enemy_random < 20)
			enemy[i].type = ENEMY_TYPE_NORMAL;

		else if (enemy_random >= 20 && enemy_random < 40)
			enemy[i].type = ENEMY_TYPE_FAST;

		else if (enemy_random >= 40 && enemy_random < 55)
			enemy[i].type = ENEMY_TYPE_TURBO;

		else if (enemy_random >= 55 && enemy_random < 70)
			enemy[i].type = ENEMY_TYPE_TRICKY;

		else if (enemy_random >= 70 && enemy_random < 85)
			enemy[i].type == ENEMY_TYPE_EDDY;

		else if (enemy_random >= 85)
			enemy[i].type = ENEMY_TYPE_BOBM;

		/*if (enemy_random < 50)
			enemy[i].type = ENEMY_TYPE_NORMAL;
		else 
			enemy[i].type = ENEMY_TYPE_BOBM;*/
	}
	else
	{
		enemy[i].hp = 10;
		enemy[i].type = ENEMY_TYPE_BOSS;
		enemy[i].scale = { 3, 3 };
	}
	
//生成場所
	enemy[i].pos.x = 1920.0f + 50.0f;
	enemy[i].pos.y = rand() % 648;
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
				enemy[i].scale.x, enemy[i].scale.y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);
		}
		else if (enemy[i].type == ENEMY_TYPE_BOBM)
		{
		    sprite_render(
				sprEnemy,
				enemy[i].pos.x, enemy[i].pos.y,
				enemy[i].scale.x, enemy[i].scale.y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, 0.8);
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
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);
		}
	}

	debug::setString("enemy_boss_timer:%d", enemy_boss_timer);

}