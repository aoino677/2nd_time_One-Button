#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include <vector>
#include <cstdlib>

Sprite* enemy_sprite[ENEMY_NUMBER];
Sprite* sprCrayon;
Sprite* sprBall;
Sprite* sprBalloon;
Sprite* sprPencil;
Sprite* sprBobm;
Sprite* sprEnemy_hp;
OBJ2D enemy[ENEMY_NUMBER];

// state類
int enemy_state;
int boss_state;

// タイマー類
int enemy_timer;
int enemy_boss_timer;
int enemy_turbo_timer[ENEMY_NUMBER];
int enemy_attack_timer[ENEMY_NUMBER];
int enemy_anim_timer[ENEMY_NUMBER];
int enemy_jump_timer[ENEMY_NUMBER];

// 強化類
float enemy_hp_up = 0.0f;       // HP増加量
float enemy_speed_up = 0.0f;    // スピード増加量
float enemy_attack_up = 0.0f;   // 攻撃増加量

// HPゲージ類
int enemy_max_hp[ENEMY_NUMBER];       // HP最大量
float enemy_hp_bar_max[ENEMY_NUMBER]; // 最大HPゲージ
float enemy_hp_rate[ENEMY_NUMBER];    // HP割合

// エネミー情報
int enemy_max;                      // 最大リスポーン数
int enemy_type_render[ENEMY_NUMBER];
int enemy_anim[ENEMY_NUMBER];
float angle[ENEMY_NUMBER];          // 角度
float enemy_border[ENEMY_NUMBER];   // 最大移動値
float enemy_speed[ENEMY_NUMBER];    // スピード
float enemy_target_y[ENEMY_NUMBER];

// エネミー初期化
void enemy_init()
{
	enemy_state = 0;
	boss_state = 0;
	enemy_timer = 0;
	enemy_boss_timer = 0;
	enemy_max = 4;

	enemy_hp_up = 0.0f;
	enemy_speed_up = 0.0f;
	enemy_attack_up = 0.0f;

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		enemy_border[i] = 0.0f;
		enemy_attack_timer[i] = 0;
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
		sprBall = sprite_load(L"Data/Images/ball.png");
		/*sprBalloon = sprite_load(L"Data/Images/crayon.png");
		sprPencil= sprite_load(L"Data/Images/crayon.png");
		sprBobm = sprite_load(L"Data/Images/crayon.png");*/
		sprEnemy_hp = sprite_load(L"Data/Images/enenmy_hp.png");

		enemy_state++;
		break;

	case 1:
		// パラメーターの設定と初期出現
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			enemy[i].scale = { ENEMY_SCALE, ENEMY_SCALE };
			enemy[i].texSize = { ENEMY_TEX_W, ENEMY_TEX_H };
			enemy[i].color = { 1, 1, 1, 1 };

			// 初期位置にランダムにスポーン
			enemy_respawn(i);
		}

		enemy_state++;
		break;

	case 2:
		for (int i = 0; i < enemy_max; i++)
		{
			// エネミーの動き
			if (enemy[i].pos.x > enemy_border[i] || enemy[i].pos.x > 530)
				enemy_move(i);
			else
				enemy_attack_action(i);

			// HPが0になった時に別の敵として再出現させる
			if (enemy[i].hp <= 0)
			{
				enemy_respawn(i);
				
				/*if (enemy_boss_timer < 600)
				{
					enemy_respawn(i);
				}
				else
				{
					enemy_boss_respawn(i);
					enemy_boss_timer = 0;
					boss_state++;
				}*/
			}
			enemy_anim_timer[i]++;
		}

		enemy_timer++;
		enemy_boss_timer++;
		

		//時間経過による敵の強化
		enemy_powerup();

		break;
	}
}

//敵を画面外のランダムな位置に出現させる

void enemy_respawn(int i)
{
	enemy_type_render[i] = ENEMY;

    //敵の選別
	int enemy_random = rand() % 20;
    
	//敵の選別(敵の種類・HP・攻撃力・スピード)
    if (enemy_random < 20)
    {
		enemy[i].type = ENEMY_TYPE_CRAYON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 1);
		//enemy_anim[i] = rand() % 120 + 600;
		enemy_anim[i] = 120;
		enemy[i].scale = { 1,1 };
		enemy[i].pivot = { 20,40 };
	}
    else if (enemy_random >= 20 && enemy_random < 40)
    {
		enemy[i].type = ENEMY_TYPE_BOLL;
		enemy_sprite[i] = sprBall;
		enemy_setting(i, 3, 1, 3);
		enemy[i].scale = { 1,1 };
		enemy[i].pivot = { 20,40 };
	}
	else if (enemy_random >= 40 && enemy_random < 60)
	{
		enemy[i].type = ENEMY_TYPE_BALLOON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 1);
		enemy[i].scale = { 1,1 };
		enemy[i].pivot = { 20,40 };
	}
	else if (enemy_random >= 60 && enemy_random < 80)
	{
		enemy[i].type = ENEMY_TYPE_PENCIL;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 1);
		enemy[i].scale = { 1,1 };
		enemy[i].pivot = { 20,40 };
	}
    else if (enemy_random >= 80)
	{
		enemy[i].type = ENEMY_TYPE_BOBM;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 1);
		enemy[i].scale = { 1,1 };
		enemy[i].pivot = { 20,40 };
	}
	
	//生成場所
	if (enemy[i].type == ENEMY_TYPE_PENCIL)
	{
		enemy[i].pos.x = SCREEN_W + 50.0f;
		enemy[i].pos.y = rand() % 250 + 70;
	}
	else if (enemy[i].type == ENEMY_TYPE_BALLOON)
	{
		enemy[i].pos.x = rand() % 400 + 1000;
		enemy[i].pos.y = -50.0f;
		enemy_target_y[i] = (float)(rand() % 400 + 600);
	}
	else
	{
		enemy[i].pos.x = SCREEN_W + 50.0f;
		enemy[i].pos.y = rand() % 724 + 356;
	}
}


//ボス生成
void enemy_boss_respawn(int i)
{
	enemy_type_render[i] = BOSS;

	switch (boss_state)
	{
	case 0://ボスクレヨン
		enemy[i].type = BOSS_TYPE_CRAYON;
		enemy_setting(i, 40, 3, 4);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };

	case 1://ミニカー
		enemy[i].type = BOSS_TYPE_MINICAR;
		enemy_setting(i, 40, 3, 10);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };

	case 2://ぬいぐるみ
		enemy[i].type = BOSS_TYPE_DOLL;
		enemy_setting(i, 40, 3, 4);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };

	case 3://ロボット
		enemy[i].type = BOSS_TYPE_ROBOT;
		enemy_setting(i, 40, 3, 4);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };

	case 4://木馬
		enemy[i].type = BOSS_TYPE_HORSE;
		enemy_setting(i, 40, 3, 4);
		enemy[i].pivot = { 60,120 };

	case 5://ロケット鉛筆
		enemy[i].type = BOSS_TYPE_PENCIL;
		enemy_setting(i, 40, 3, 4);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };

	case 6://ショベルカー
		enemy[i].type = BOSS_TYPE_EXCAVATOR;
		enemy_setting(i, 40, 3, 4);
		enemy[i].scale = { 3,3 };
		enemy[i].pivot = { 60,120 };
	}


	enemy[i].pos.x = SCREEN_W + 200.0f;
	enemy[i].pos.y = SCREEN_H * 0.9f;
}


// 敵の移動と種類ごとの能力
void enemy_move(int i)
{
	// それぞれの最大移動値を計算
	enemy_border[i] = 280 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;

	// 通常の敵 / クレヨン
	if (enemy[i].type == ENEMY_TYPE_CRAYON)
	{
		enemy[i].pos.x -=enemy[i].moveSpeed;
		/*if (enemy_anim_timer[i] == enemy_anim[i])
		{
			enemy_anim_timer[i] = 0;
			while (enemy_jump_timer[i]<30)
			{
				enemy_jump_timer[i]++;
				if (enemy_jump_timer[i] < 20)
					enemy[i].pos.y += 0.2;
				else
					enemy[i].pos.y += 0.2;
			}
			enemy_jump_timer[i] = 0;
		}*/
	}
	// 速い敵
	else if (enemy[i].type == ENEMY_TYPE_BOLL)
	{
		enemy[i].pos.x -= enemy[i].moveSpeed;
	}
	// 風船
	else if (enemy[i].type == ENEMY_TYPE_BALLOON)
	{
		if (enemy[i].pos.y < enemy_target_y[i])
		{
			enemy[i].pos.y += enemy[i].moveSpeed;
		}
		else
		{
			enemy[i].pos.x -= enemy[i].moveSpeed;
		}
	}
	// ロケット鉛筆
	else if (enemy[i].type == ENEMY_TYPE_PENCIL)
	{
		enemy[i].pos.x -= enemy[i].moveSpeed;
	}
	// ボム敵
	else if (enemy[i].type == ENEMY_TYPE_BOBM)
	{
		enemy[i].pos.x -= enemy[i].moveSpeed;

		if (enemy[i].hp <= 0)
		{
			for (int j = 0; j < enemy_max; j++)
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
	// ボス敵
	else if (enemy[i].type == BOSS_TYPE_CRAYON)
	{
		enemy[i].pos.x -= enemy[i].moveSpeed;
	}
}

//敵の設定(番号・HP・攻撃力・スピード)
void enemy_setting(int i,int hp,int attack,float speed)
{
	enemy_max_hp[i] = hp + enemy_hp_up;
	enemy[i].hp = enemy_max_hp[i];
	enemy[i].attack = attack + enemy_attack_up;
	enemy[i].moveSpeed = speed + enemy_speed_up;
}



//攻撃判定
void enemy_attack_action(int i)
{
	enemy_attack_timer[i]++;

	if (enemy_attack_timer[i] < 15)
		enemy[i].pos.x -= 3.0f;
	else if (enemy_attack_timer[i] == 15)
	{
		player_hp--;
		shake(30, 30);
	}
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
		if (enemy_max + 3 <= ENEMY_NUMBER)
		    enemy_max += 3;
		else
			enemy_max = ENEMY_NUMBER;
		
		enemy_hp_up += 10.0f;
		enemy_speed_up += 0.5f;
		enemy_timer = 0;
	}
}

void enemy_render()
{
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].hp <= 0) { continue; }
		
		// 敵の種類ごとの描画分岐
		if (enemy_type_render[i] == BOSS)
		{
			sprite_render(
				sprCrayon,
				enemy[i].pos.x, enemy[i].pos.y,
				enemy[i].scale.x, enemy[i].scale.y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z, enemy[i].color.w);

			// HP割合
			enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
			if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
			if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;
			enemy_hp_bar_max[i] = 200.0f * enemy_hp_rate[i];

			// ダメージ量
			primitive::rect(enemy[i].pos.x - 220, enemy[i].pos.y - 400,
				200, 20,
				0, 0, 0,
				1, 0, 0);

			// 残りHP
			primitive::rect(
				enemy[i].pos.x - 220, enemy[i].pos.y - 400,
				enemy_hp_bar_max[i], 20,
				0, 0, 0,
				0, 1, 0);
		}
		else if (enemy_type_render[i] == ENEMY)
		{
			sprite_render(
				enemy_sprite[i],
				enemy[i].pos.x, enemy[i].pos.y,
				enemy[i].scale.x, enemy[i].scale.y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0));

			// HP割合
			enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
			if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
			if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];

			// ダメージ量
			primitive::rect(enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				80, 10,
				0, 0, 0,
				1, 0, 0);

			// 残りHP（緑）
			primitive::rect(
				enemy[i].pos.x - 40, enemy[i].pos.y - 60,
				enemy_hp_bar_max[i], 10,
				0, 0, 0,
				0, 1, 0);

			// HP枠
			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 43, enemy[i].pos.y - 63);
		}
	}

	debug::setString("enemy_boss_timer:%d", enemy_boss_timer);
	debug::setString("boss_state:%d", boss_state);
	debug::setString("enemy_anim_timer[0]:%d", enemy_anim_timer[0]);
}