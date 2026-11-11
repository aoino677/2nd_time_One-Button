#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cmath>

Sprite* sprCrayon;
Sprite* sprEnemy_hp;
OBJ2D enemy[ENEMY_NUMBER];

int enemy_state;

//タイマー類
int enemy_timer;
int enemy_boss_timer;
//int damage_timer[ENEMY_NUMBER];
int enemy_turbo_timer[ENEMY_NUMBER];
int enemy_attack_timer[ENEMY_NUMBER];

//強化類
float enemy_hp_up = 0.0f;              //HP増加量
float enemy_speed_up = 0.0f;           //スピード増加量
float enemy_attack_up = 0.0f;          //攻撃増加量

//HPゲージ類
int enemy_max_hp[ENEMY_NUMBER];        //HP最大量
float enemy_hp_bar_max[ENEMY_NUMBER];  //最大HPゲージ
float enemy_hp_rate[ENEMY_NUMBER];     //HP割合

//エネミー情報
int enemy_max;                         //最大リスポーン数
float angle[ENEMY_NUMBER];             //角度
float enemy_border[ENEMY_NUMBER];      //最大移動値
float enemy_speed[ENEMY_NUMBER];       //スピード


//エネミー初期化
void enemy_init()
{
	enemy_state = 0;
	enemy_timer = 0;
	enemy_boss_timer = 0;
	enemy_max = 3; 

	enemy_hp_up = 0.0f;
	enemy_speed_up = 0.0f;
	enemy_attack_up = 0.0f;

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		enemy_border[i] = 0.0f;
		enemy_attack_timer[i] = 0;
		//damage_timer[i] = 0;
		enemy_turbo_timer[i] = 0;
	}
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
			if (enemy[i].pos.x > enemy_border[i])
				enemy_move(i);
			else
				enemy_attack_action(i);

			//HPが0になった時に別の敵として再出現させる
			if (enemy[i].hp <= 0)
			{
				enemy_respawn(i);

				if (enemy[i].type == ENEMY_TYPE_BOSS)
					enemy_boss_timer = 0;
			}

			enemy_turbo_timer[i]++;
		}

		//時間ごとに強化
		enemy_powerup();
		enemy_timer++;

		//ボスタイマー
		enemy_boss_timer++;

		break;
	}
}


//敵を画面外のランダムな位置に出現させる
void enemy_respawn(int i)
{
	enemy[i].move_timer = 0; //タイマーをリセット

	//敵の選別
	if (enemy_boss_timer < 3200)
	{
		int enemy_random = rand() % 100;

		//敵の選別(敵の種類・最大HP・HP・中心点)
		if (enemy_random < 50)
		{
			enemy[i].type = ENEMY_TYPE_NORMAL;
			enemy_max_hp[i] = 20+enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}

		/*else if (enemy_random >= 20 && enemy_random < 40)
		{
			enemy[i].type = ENEMY_TYPE_FAST;
			enemy_max_hp[i] = 20 + enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}

		else if (enemy_random >= 40 && enemy_random < 55)
		{
			enemy[i].type = ENEMY_TYPE_TURBO;
			enemy_max_hp[i] = 20 + enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}

		else if (enemy_random >= 55 && enemy_random < 70)
		{
			enemy[i].type = ENEMY_TYPE_TRICKY;
			enemy_max_hp[i] = 20 + enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}
		else if (enemy_random >= 70 && enemy_random < 85)
		{
			enemy[i].type = ENEMY_TYPE_EDDY;
			enemy_max_hp[i] = 20 + enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}*/
		else if (enemy_random >= 50)
		{
			enemy[i].type = ENEMY_TYPE_BOBM;
			enemy_max_hp[i] = 60 + enemy_hp_up;
			enemy[i].hp = enemy_max_hp[i];
			enemy[i].attack = 1 + enemy_attack_up;
			enemy[i].pivot = { 20,40 };
		}
	}
	else
	{
		enemy[i].type = ENEMY_TYPE_BOSS;
		enemy[i].pivot = { 20, 40 };
		enemy_max_hp[i] = (int)(100 + enemy_hp_up); 
		enemy[i].hp = enemy_max_hp[i];
		enemy[i].attack = (int)(5 + enemy_attack_up);
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
		enemy[i].pos.y = (float)(rand() % 724 + 356);
	}
}


//敵の移動と種類ごとの能力
void enemy_move(int i)
{
	//それぞれの最大移動値を計算
	enemy_border[i] = 280 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;

	//角度を計算
	float base_angle = atan2f(540 - enemy[i].pos.y, -200.0f - enemy[i].pos.x);

	//通常の敵
	if (enemy[i].type == ENEMY_TYPE_NORMAL)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		//動き
		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];
	}
	//速い敵
	else if (enemy[i].type == ENEMY_TYPE_FAST)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 2;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 2;
	}
	//一定の間隔で進む敵
	else if (enemy[i].type == ENEMY_TYPE_TURBO)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		if (enemy_turbo_timer[i] < 15)
		{
			enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 5;
			enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 5;
		}
		else if (enemy_turbo_timer[i] > 100)
			enemy_turbo_timer[i] = 0;
	}
	//揺れ動く敵
	else if (enemy[i].type == ENEMY_TYPE_TRICKY)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		enemy[i].move_timer++;

		float wobble = sinf(enemy[i].move_timer * 0.15f) * 20.0f;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];
		enemy[i].pos.x += -sinf(base_angle) * wobble;
		enemy[i].pos.y += cosf(base_angle) * wobble;
	}
	//渦巻く敵
	else if (enemy[i].type == ENEMY_TYPE_EDDY)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];
		enemy[i].pos.x += -sinf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += cosf(base_angle) * enemy_speed[i];
	}
	//ボム敵
	else if (enemy[i].type == ENEMY_TYPE_BOBM)
	{
		//スピード
		enemy_speed[i] = 3.0f + enemy_speed_up;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i];
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i];

		if (enemy[i].hp <= 0)
		{
			for (int j = 0; j < ENEMY_NUMBER; j++)
			{
				if (i == j) continue;

				float enemy_Vec_x = enemy[j].pos.x - enemy[i].pos.x;
				float enemy_Vec_y = enemy[j].pos.y - enemy[i].pos.y;
				float enemy_Vec = sqrtf(enemy_Vec_x * enemy_Vec_x + enemy_Vec_y * enemy_Vec_y);

				if (enemy_Vec < 100.0f)
					enemy[j].hp -= 10;
			}
		}
	}
	//ボス敵
	else if (enemy[i].type == ENEMY_TYPE_BOSS)
	{
		enemy_speed[i] = 3.0f + enemy_speed_up;

		enemy[i].pos.x += cosf(base_angle) * enemy_speed[i] * 0.5f;
		enemy[i].pos.y += sinf(base_angle) * enemy_speed[i] * 0.5f;
	}
}


//攻撃判定
void enemy_attack_action(int i)
{
	enemy_attack_timer[i]++;

	if (enemy_attack_timer[i] < 15)
		enemy[i].pos.x -= 3.0f;
	else if (enemy_attack_timer[i] == 15)
		player_hp -= enemy[i].attack;
	else if (enemy_attack_timer[i] > 15 && enemy_attack_timer[i] < 45)
		enemy[i].pos.x += 1.5f;
	else if (enemy_attack_timer[i] > 105)
		enemy_attack_timer[i] = 0;
}


//時間ごとに強化
void enemy_powerup()
{
	if (enemy_timer > 1200)
	{
		enemy_max += 3;
		if (enemy_max > ENEMY_NUMBER) enemy_max = ENEMY_NUMBER; 

		enemy_hp_up += 10.0f;
		enemy_attack_up += 10.0f;
		enemy_speed_up += 1.5f;
		enemy_timer = 0;
	}
}



void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].hp <= 0) { continue; }

		// ゼロ除算防止
		if (enemy_max_hp[i] <= 0) enemy_max_hp[i] = 1;

		//HP割合
		enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
		if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
		if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;
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

		// 敵の種類ごとの描画分岐
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
		else if (enemy[i].type == ENEMY_TYPE_BOBM)
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
		}
		else
		{
			//通常やその他の敵画像
			sprite_render(
				sprCrayon,
				enemy[i].pos.x, enemy[i].pos.y,
				1, 1,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				1.0f, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);
		}
	}
}
