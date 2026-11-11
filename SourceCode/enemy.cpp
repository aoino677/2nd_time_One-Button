#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include <vector>
#include <cstdlib>

Sprite* sprCrayon;
Sprite* sprEnemy_hp;
OBJ2D enemy[ENEMY_NUMBER];
int enemy_state;
int enemy_timer;
int enemy_max;
int enemy_boss_timer;
int enemy_attack_timer[ENEMY_NUMBER];
int damage_timer[ENEMY_NUMBER];
int enemy_turbo[ENEMY_NUMBER];
int enemy_max_hp[ENEMY_NUMBER];
float angle[ENEMY_NUMBER];
float pos[ENEMY_NUMBER];
float enemy_border[ENEMY_NUMBER];
float enemy_speed[ENEMY_NUMBER];
float enemy_hp_rate[ENEMY_NUMBER];
float enemy_hp_bar_max[ENEMY_NUMBER];


//エネミー初期化
void enemy_init()
{
	
	enemy_state = 0;
	enemy_timer = 0;
	enemy_boss_timer = 0;
	enemy_max = 3;
	pos[ENEMY_NUMBER] = { 0 };
	
	
}


void enemy_deinit()
{
	safe_delete(sprCrayon);
	safe_delete(sprEnemy_hp);
}


void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		// スプライトの読み込み
		sprCrayon = sprite_load(L"Data/Images/crayon.png");
		sprEnemy_hp = sprite_load(L"Data/Images/enenmy_hp.png");

		enemy_state++;
		break;

	case 1:
		//パラメーターの設定と初期出現
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			enemy[i].scale = { ENEMY_SCALE , ENEMY_SCALE };
			enemy[i].texSize = { ENEMY_TEX_W , ENEMY_TEX_H };
			//enemy[i].pivot = { ENEMY_PIVOT_X , ENEMY_PIVOT_Y };
			enemy[i].color = { 1, 0, 0, 1 };

			//初期位置にランダムにスポーン
			enemy_respawn(i);
		}

		enemy_state++;
		break;

	case 2:
		for (int i = 0; i < enemy_max; i++)
		{
			//エネミーの動き
			if (enemy[i].pos.x > pos[i])
				enemy_ability(i);
			else
				enemy_attack(i);

			//HPが0になった時に別の敵として再出現させる
			if (enemy[i].hp <= 0)
			{
				enemy_respawn(i);

				if (enemy[i].type == ENEMY_TYPE_BOSS)
				     enemy_boss_timer = 0;
			}
		    
			enemy_turbo[i]++;
		}
		
		
		//秒数がたったら強化
		if (enemy_timer> 1200)
		{
			enemy_max++;
			//enemy_hp[i]++;
			enemy_timer = 0;
		}

		enemy_timer++;
		enemy_boss_timer++;
		
		break;
	}
}


//敵を画面外のランダムな位置に出現させる
void enemy_respawn(int i)
{
	enemy[i].move_timer = 0; //タイマーをリセッ

	//敵の選別
	if (enemy_boss_timer < 3200)
	{
		int enemy_random = rand() % 100;

		//敵の選別(敵の種類・HP・中心点)
		if (enemy_random < 50)
		{
			enemy[i].type = ENEMY_TYPE_NORMAL;
			enemy_max_hp[i] = 20;
			enemy[i].hp = 2;
			enemy[i].pivot = { 20,40 };
		}

		/*else if (enemy_random >= 20 && enemy_random < 40)
		{
			enemy[i].type = ENEMY_TYPE_FAST;
			enemy[i].hp = 3;
		}

		else if (enemy_random >= 40 && enemy_random < 55)
		{
			enemy[i].type = ENEMY_TYPE_TURBO;
			enemy[i].hp = 3;
		}

		else if (enemy_random >= 55 && enemy_random < 70)
		{
			enemy[i].type = ENEMY_TYPE_TRICKY;
			enemy[i].hp = 3;
		}
		else if (enemy_random >= 70 && enemy_random < 85)
		{
			enemy[i].type = ENEMY_TYPE_EDDY;
			enemy[i].hp = 3;
		}*/
		else if (enemy_random >= 50)
		{
			enemy[i].type = ENEMY_TYPE_BOBM;
			enemy_max_hp[i] = 20;
			enemy[i].hp = 20;
			enemy[i].pivot = { 20,40 };
		}
	}
	else
	{
		enemy[i].type = ENEMY_TYPE_BOSS;
		enemy[i].hp = 10;
	}

	//ボスの生成場所
	if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		enemy[i].pos.x = SCREEN_W + 200.0f;
		enemy[i].pos.y = SCREEN_H * 0.5f;
	}
	//ボス以外の生成場所
	else
	{
		enemy[i].pos.x = SCREEN_W + 50.0f;
		enemy[i].pos.y = rand() % 724 + 356;
		
		/*int enemy_respawn_place = rand() % 2;
		if (enemy_respawn_place == 0)
		{
			enemy[i].pos.x = SCREEN_W + 50.0f;
			enemy[i].pos.y = rand() % 724+356;
		}
		else if (enemy_respawn_place == 1)
		{
			enemy[i].pos.x = rand() % 640 + 1280.0f;
			enemy[i].pos.y = SCREEN_H + 100.0f;
		}*/
	}
}


//敵の移動と種類ごとの能力
void enemy_ability(int i)
{
	//それぞれの最大移動値を計算
	pos[i]= 280 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;
	
	//角度を計算
    float base_angle = atan2f(540 - enemy[i].pos.y, -200.0f - enemy[i].pos.x);
	
	//通常の敵
	if (enemy[i].type == ENEMY_TYPE_NORMAL)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];
	}

	//速い敵
	if (enemy[i].type == ENEMY_TYPE_FAST)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 2;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 2;
	}

	//一定の間隔で進む敵
	else if (enemy[i].type == ENEMY_TYPE_TURBO)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		//動き
		if (enemy_turbo[i] < 15)
		{
			enemy_speed[i] = 3.0f;
			enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 5;
			enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 5;
		}
		else if (enemy_turbo[i] > 100)
			enemy_turbo[i] = 0;
	}

	//揺れ動く敵
	else if (enemy[i].type == ENEMY_TYPE_TRICKY)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		enemy[i].move_timer++;

		//サイン波を使って左右に揺れる幅を計算
		float wobble = sinf(enemy[i].move_timer * 0.15f) * 20.0f;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * wobble;
		enemy[i].pos.y += cosf(base_angle) * wobble;
	}

	//渦巻く敵
	else if (enemy[i].type == ENEMY_TYPE_EDDY)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];

		//進行方向に対して垂直な方向に揺れを足す
		enemy[i].pos.x += -sinf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += cosf(base_angle) * enemy_speed[i];
	}

	//ボム敵
	else if (enemy[i].type == ENEMY_TYPE_BOBM)
	{
		enemy_speed[i] = 3.0f;
			
		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];

		if (enemy[i].hp <= 0)
		{
			for (int j = 0; j < ENEMY_NUMBER; j++)
			{
				if (i == j) continue;

				//それぞれのベクトルを計算
				float enemy_Vec_x = enemy[j].pos.x - enemy[i].pos.x;
				float enemy_Vec_y = enemy[j].pos.y - enemy[i].pos.y;
				float enemy_Vec = sqrtf(enemy_Vec_x * enemy_Vec_x + enemy_Vec_y * enemy_Vec_y);

				//爆発に範囲
				if (enemy_Vec < 100.0f)
					enemy[j].hp -= 10;
			}
		}
	}

	//ボス敵
	else if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		//スピード
		enemy_speed[i] = 3.0f;
			
		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 0.5f;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 0.5f;
	}
	
}


//攻撃判定
void enemy_attack(int i)
{
	enemy_attack_timer[i]++;

	if (enemy_attack_timer[i] < 15)
		enemy[i].pos.x -= 3.0f;

	else if (enemy_attack_timer[i] == 15)
		player_hp--;

	else if (enemy_attack_timer[i] > 15 && enemy_attack_timer[i] < 45)
		enemy[i].pos.x += 1.5f;

	else if (enemy_attack_timer[i] > 105)
		enemy_attack_timer[i] = 0;
}

//時間ごとに強化



void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{ 
		if (enemy[i].hp <= 0) { continue; }

	    

		if (enemy[i].type == ENEMY_TYPE_BOSS)
		{
			
			
			sprite_render(
				sprCrayon,
				enemy[i].pos.x, enemy[i].pos.y,
				3, 3,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);
		}
		if (enemy[i].type == ENEMY_TYPE_BOBM)
		{
			sprite_render(
				sprCrayon,
				enemy[i].pos.x, enemy[i].pos.y,
				1, 1,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				0, enemy[i].color.y, 1, enemy[i].color.w);
			
			//HP割合
			enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];

			//ダメージ量
			primitive::rect(enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				80, 10,
				0, 0,
				0,
				1, 0, 0);

			//残りHP
			primitive::rect(
				enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				enemy_hp_bar_max[i], 10,
				0, 0,
				0,
				0, 1, 0);

			//HP枠
			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 43, enemy[i].pos.y - 63);
		}
		else
		{
			//敵画像
			sprite_render(
				sprCrayon,
				enemy[i].pos.x, enemy[i].pos.y,
				1, 1,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				1.0f, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);

			//HP割合
			enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];

			//ダメージ量
			primitive::rect(enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				80, 10,
				0, 0,
				0,
				1, 0, 0);

			//残りHP
			primitive::rect(
				enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				enemy_hp_bar_max[i], 10,
				0, 0,
				0,
				0, 1, 0);

			//HP枠
			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 43, enemy[i].pos.y - 63);
		}
	}

	debug::setString("enemy_boss_timer:%d", enemy_boss_timer);

}