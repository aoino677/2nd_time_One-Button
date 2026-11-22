#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include "bullet.h"
#include "AttackCollision.h"
#include "item.h"
#include "effect_burst.h"
#include <vector>
#include <cstdlib>
#include <cmath>

//enemy構造体
OBJ2D enemy[ENEMY_NUMBER];

TireEffect tire_particles[TIRE_PARTICLE_NUMBER];

//難易度（0:EASY, 1:NORMAL, 2:HARD）
int difficulty = 1;

//敵画像
Sprite* enemy_sprite[ENEMY_NUMBER];
Sprite* sprEnemyCrayon;                      //クレヨン画像
Sprite* sprEnemyBall;                        //ボール画像
Sprite* sprEnemyRocket;                      //ロケット画像
Sprite* sprEnemyCar;                         //カー画像
Sprite* sprEnemyBear;                        //クマ画像

//敵サブ画像
Sprite* sprBalloon;
Sprite* sprShadow;                      //ボール影
Sprite* sprTirefront;                   //前輪
Sprite* sprTireback;                    //後輪
Sprite* sprWarning;

//HP枠
Sprite* sprEnemy_hp;                    //敵HP枠
Sprite* sprBoss_hp;                     //ボスHP枠

// state類
int enemy_state;
int boss_state;

// タイマー類
int enemy_powerup_timer;               //強化タイマー
int enemy_boss_timer;                  //ボスタイマー
int crayon_spawn_timer;                //クレヨンタイマー
int enemy_attack_timer[ENEMY_NUMBER];  //攻撃タイマー
int enemy_anim_timer[ENEMY_NUMBER];    //アニメーションタイマー

//被弾・ノックバック用変数
int enemy_damage_timer[ENEMY_NUMBER];         //通常被弾時の赤色点滅用タイマー
int enemy_knockback_timer[ENEMY_NUMBER];      //HP半分以下のノックバック用タイマー
bool enemy_half_knockback_done[ENEMY_NUMBER]; //HP半分以下のノックバックが完了したか
int enemy_prev_hp[ENEMY_NUMBER];              //ダメージ検知用の前回HP記憶配列
float hit_buck;                               //ノックバックをプラスする数値

//強化類
float enemy_hp_up;               //HP増加量
float enemy_speed_up;            //スピード増加量
float enemy_attack_up;           //攻撃増加量

//HPゲージ類
int enemy_max_hp[ENEMY_NUMBER];        //HP最大量
float enemy_hp_bar_max[ENEMY_NUMBER];  //最大HPゲージ
float enemy_hp_rate[ENEMY_NUMBER];     //HP割合

//エネミー情報
int enemy_kill_count;
int enemy_max;                         //最大リスポーン数
int enemy_type_render[ENEMY_NUMBER];   //敵タイプ描画
float enemy_border[ENEMY_NUMBER];      //最大移動値
float enemy_target_y[ENEMY_NUMBER];    //着地地点
float balloon_pos_x[ENEMY_NUMBER];     //バルーン位置X
float balloon_pos_y[ENEMY_NUMBER];     //バルーン位置Y

float tire_angle[ENEMY_NUMBER];
float gravity = 0.5f;

//難易度
int prob_ball;
int prob_balloon;
int prob_rocket;
int prob_bomb;


//仮変数
bool respawn;
float x_1 = 0;
float x_2 = 1915;
float etimer = 300;
float effect_boss_clear = 0.0f;
float effect_boss_timer = 0.0f;
float effect_boss_wave = 0.0f;
float swing_speed;
int robot_wait_timer = 0;

//クマの移動変数
int bear_land_wait_max = 90;           //着地して溜める時間
int bear_land_wait = 0;                //着地後の待機フレーム数
float bear_ground_y = SCREEN_H * 0.5f; //着地・バウンド基準の地面の高さ
float bear_vy = 0.0f;                  //縦方向の速度
float bear_drop_gravity = 0.95f;       //降下時の加速（重め）
float bear_jump_gravity = 0.28f;       //跳ね移動時の重力（ゆったり）
float bear_jump_power = -6.5f;         //跳ね初速（低め・重め）
bool bear_dropping = true;             //上から降下中かどうか



//エネミー初期化
void enemy_init()
{
	particles_init();

	enemy_kill_count = 0;

	enemy_state = 0;
	boss_state = 0;
	enemy_powerup_timer = 0;
	enemy_boss_timer = 0;
	crayon_spawn_timer = 0;
	enemy_max = 3;

	enemy_hp_up = 0.0f;
	enemy_speed_up = 0.0f;
	enemy_attack_up = 0.0f;
	hit_buck = 0.0f;
	respawn = false;

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		enemy[i].hp = 0;
		enemy[i].attack = 0;
		enemy[i].moveSpeed = 0;
		enemy_border[i] = 0.0f;
		enemy_attack_timer[i] = 0;
		enemy_anim_timer[i] = 0;
		enemy_damage_timer[i] = 0;
		enemy_knockback_timer[i] = 0;
		enemy_half_knockback_done[i] = false;
		enemy_prev_hp[i] = 0;
		enemy[i].type = ENEMY_TYPE_NONE;
		enemy[i].pos = { 0.0f, 0.0f };
	}

	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++)
	{
		tire_particles[i].active = false;
	}
}

void enemy_deinit()
{
	safe_delete(sprEnemyCrayon);
	safe_delete(sprEnemyBall);
	safe_delete(sprShadow);
	safe_delete(sprEnemyRocket);
	safe_delete(sprEnemyCar);
	safe_delete(sprTirefront);
	safe_delete(sprTireback);
	safe_delete(sprEnemy_hp);
	safe_delete(sprBoss_hp);
	safe_delete(sprWarning);
	safe_delete(sprBalloon);
}

void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		sprEnemyCrayon = sprite_load(L"Data/Images/crayon.png");
		sprEnemyBall = sprite_load(L"Data/Images/ball.png");
		sprEnemyRocket = sprite_load(L"Data/Images/rocket.png");
		sprEnemyCar = sprite_load(L"Data/Images/car.png");
		sprEnemyBear = sprite_load(L"Data/Images/bear.png");

		sprShadow = sprite_load(L"Data/Images/ball_shadow.png");

		sprTirefront = sprite_load(L"Data/Images/tire_front.png");
		sprTireback = sprite_load(L"Data/Images/tire_back.png");
		sprEnemy_hp = sprite_load(L"Data/Images/enemy_hp.png");
		sprBoss_hp = sprite_load(L"Data/Images/boss_hp.png");
		sprWarning = sprite_load(L"Data/Images/warning.png");
		sprBalloon = sprite_load(L"Data/Images/balloon.png");

		enemy_state++;
		break;

	case 1:
		for (int i = 0; i < enemy_max; i++)
		{
			enemy[i].color = { 1, 1, 1, 1 };

			enemy_respawn(i);
		}

		//クレヨン隊列
		enemy_crayon_respawn();

		enemy_state++;
		break;

	case 2:
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			//HPが0以下になった瞬間(死亡＆パーティクル発生)
			if (enemy[i].hp <= 0 && enemy_prev_hp[i] > 0)
			{
				enemy_kill_count++;

				item_drop(i);
				if (enemy_type_render[i] == BOSS)
				{
					particles_update(enemy[i].pos.x, enemy[i].pos.y, 8, 1.0f, 0.6f, 0.1f);
					particles_update(enemy[i].pos.x, enemy[i].pos.y, 8, 1.0f, 1.0f, 1.0f);

					if (enemy[i].type == BOSS_TYPE_CAR)
						tires_move(enemy[i].pos.x, enemy[i].pos.y);
				}
				else if (enemy[i].type == ENEMY_TYPE_BOBM)
					particles_update(enemy[i].pos.x, enemy[i].pos.y, 15, 1.0f, 0.2f, 0.2f);
				else if (enemy[i].type == ENEMY_TYPE_CRAYON)
					particles_update(enemy[i].pos.x, enemy[i].pos.y, 8, 1.0f, 0.0f, 0.0f);
				else
					particles_update(enemy[i].pos.x, enemy[i].pos.y, 8, 0.0f, 0.0f, 1.0f);
			}

			//ダメージを受けた瞬間の判定(前回のHPより減っていれば被弾タイマーセット)
			if (enemy[i].hp < enemy_prev_hp[i] && enemy[i].hp > 0)
			{
				enemy_damage_timer[i] = 10;
			}

			//現在のHPを次フレームの比較用に保存
			enemy_prev_hp[i] = enemy[i].hp;

			if (enemy[i].hp <= 0)
			{
				if (enemy[i].type == ENEMY_TYPE_BOBM)
				{
					enemy_bomb_explosion(i);
					enemy[i].type = ENEMY_TYPE_NONE;
				}

				if (i < enemy_max)
				{
					enemy_respawn(i);
				}
				continue;
			}
			if (fever_blue == false)
			{
				//防衛ライン
				if (enemy[i].pos.x > enemy_border[i] || enemy[i].pos.x > 600)
				{
					//回転
					if (enemy[i].type == ENEMY_TYPE_BOLL)
						enemy[i].angle -= 0.06f;
					if (enemy[i].type == BOSS_TYPE_CAR)
						tire_angle[i] -= 0.06f;

					enemy_move(i);
				}
				else
				{
					if (enemy[i].type != ENEMY_TYPE_BOBM)
					{
						enemy_attack_action(i);
					}
				}
			}

			//ダメージ時の赤色点滅
			enemy_damage_effect(i);

			//ノックバック
			enemy_knockback_check(i);

			enemy_anim_timer[i]++;
		}

		enemy_powerup_timer++;
		enemy_boss_timer++;
		crayon_spawn_timer++;

		if (fever_blue == false)
		{
			////ボスリスポーン
			//if (enemy_boss_timer >= 200)
			//{
			//	for (int i = enemy_max; i < ENEMY_NUMBER; i++)
			//	{
			//		if (enemy[i].hp <= 0)
			//		{
			//			respawn = true;
			//			enemy_boss_respawn(i);
			//			enemy_boss_timer = 0;
			//			break;
			//		}
			//	}
			//}
			if (respawn == true)
			{
				effect_boss_respawn();
				etimer--;
			}

			//クレヨンリスポーン
			if (crayon_spawn_timer >= 200)
			{
				enemy_crayon_respawn();
				crayon_spawn_timer = 0;
			}

			//時間経過でパワーアップ
			enemy_powerup();
		}

		break;
	}
}

//難易度ごとののHP・スピード・攻撃力
void get_enemy_status_by_difficulty(int type, int& hp, float& speed, int& attack)
{
	// EASY
	if (difficulty == 0)
	{
		if (type == ENEMY_TYPE_BOLL) { hp = 2;    speed = 3.8f; attack = 1; }
		if (type == ENEMY_TYPE_BALLOON) { hp = 1;    speed = 1.5f; attack = 1; }
		if (type == ENEMY_TYPE_ROCKET) { hp = 1;    speed = 2.2f; attack = 1; }
		if (type == ENEMY_TYPE_BOBM) { hp = 2;    speed = 0.8f; attack = 1; }
		if (type == ENEMY_TYPE_CRAYON) { hp = 2;    speed = 0.8f; attack = 1; }
		if (type == BOSS_TYPE_BEAR) { hp = 450;  speed = 0.6f; attack = 1; }
	}
	// NORMAL
	else if (difficulty == 1)
	{
		if (type == ENEMY_TYPE_BOLL) { hp = 1;    speed = 10.0f; attack = 1; }
		if (type == ENEMY_TYPE_BALLOON) { hp = 1;    speed = 2.0f; attack = 1; }
		if (type == ENEMY_TYPE_ROCKET) { hp = 1;    speed = 3.0f; attack = 1; }
		if (type == ENEMY_TYPE_BOBM) { hp = 10;    speed = 1.0f; attack = 1; }
		if (type == ENEMY_TYPE_CRAYON) { hp = 1;    speed = 1.0f; attack = 1; }
		if (type == BOSS_TYPE_BEAR) { hp = 700;  speed = 0.8f; attack = 1; }
	}
	// HARD
	else if (difficulty == 2)
	{
		if (type == ENEMY_TYPE_BOLL) { hp = 5;    speed = 6.5f; attack = 2; }
		if (type == ENEMY_TYPE_BALLOON) { hp = 2;    speed = 2.8f; attack = 1; }
		if (type == ENEMY_TYPE_ROCKET) { hp = 2;    speed = 4.2f; attack = 2; }
		if (type == ENEMY_TYPE_BOBM) { hp = 5;    speed = 1.4f; attack = 2; }
		if (type == ENEMY_TYPE_CRAYON) { hp = 5;    speed = 1.4f; attack = 2; }
		if (type == BOSS_TYPE_BEAR) { hp = 1000; speed = 1.2f; attack = 2; }
	}
}

//難易度ごとの出現確率
int roll_enemy_type_by_difficulty()
{
	//EASY
	if (difficulty == 0)
	{
		prob_ball = 40;
		prob_balloon = 30;
		prob_rocket = 20;
		prob_bomb = 10;
	}
	//NORMAL
	else if (difficulty == 1)
	{
		prob_ball = 5;
		prob_balloon = 5;
		prob_rocket = 5;
		prob_bomb = 85;
	}
	//HARD
	else if (difficulty == 2)
	{
		prob_ball = 20;
		prob_balloon = 20;
		prob_rocket = 30;
		prob_bomb = 30;
	}

	int r = rand() % 100;
	if (r < prob_ball)                                return ENEMY_TYPE_BOLL;
	if (r < prob_ball + prob_balloon)                 return ENEMY_TYPE_BALLOON;
	if (r < prob_ball + prob_balloon + prob_rocket)   return ENEMY_TYPE_ROCKET;
	return ENEMY_TYPE_BOBM;
}



//敵の初期設定(番号・画像幅・中心点・スケール・角度)
void enemy_setting(int i, float a, float b, float x, float y, float n, float m, int z)
{
	int hp = 1;
	float speed = 1.0f;
	int attack = 1;

	//enemy[i].type が決まったあとに呼ぶ前提で、難易度＆敵タイプから取得
	get_enemy_status_by_difficulty(enemy[i].type, hp, speed, attack);

	enemy_max_hp[i] = hp + (int)enemy_hp_up;
	enemy[i].hp = enemy_max_hp[i];
	enemy_prev_hp[i] = enemy[i].hp;

	enemy[i].attack = attack + (int)enemy_attack_up;
	enemy_damage_timer[i] = 0;

	enemy[i].moveSpeed = speed + enemy_speed_up;
	enemy[i].texSize = { a, b };
	enemy[i].pivot = { x, y };
	enemy[i].scale = { n, m };
	enemy[i].angle = z;

	enemy_knockback_timer[i] = 0;
	enemy_half_knockback_done[i] = false;
}

//クレヨンリスポーン
void enemy_crayon_respawn()
{
	if (fever_blue == false)
	{
		int squad_max = 5;
		int spawned_count = 0;

		for (int i = enemy_max; i < ENEMY_NUMBER; i++)
		{
			if (enemy[i].hp <= 0)
			{
				int idx = i;
				bool is_bomb = (rand() % 100 < 5);

				enemy[idx].type = is_bomb ? ENEMY_TYPE_BOBM : ENEMY_TYPE_CRAYON;
				enemy_sprite[idx] = sprEnemyCrayon;
				enemy_setting(i, 64, 160, 32, 80, 1, 1, 0);

				if (is_bomb)
					enemy[idx].color = { 1.0f, 0.3f, 0.3f, 1.0f };
				else
					enemy[idx].color = { 1.0f, 1.0f, 1.0f, 1.0f };

				enemy_type_render[idx] = ENEMY;

				enemy[idx].pos.x = SCREEN_W + 180.0f - (spawned_count * 20.0f);
				enemy[idx].pos.y = 420.0f + (spawned_count * 150.0f);

				enemy_prev_hp[idx] = enemy[idx].hp;
				spawned_count++;

				if (spawned_count >= squad_max)
				{
					break;
				}
			}
		}
	}
}

//出現場所設定
void enemy_respawn(int i)
{
	if (fever_blue == false)
	{
		enemy_type_render[i] = ENEMY;
		enemy[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		enemy[i].type = roll_enemy_type_by_difficulty();

		if (enemy[i].type == ENEMY_TYPE_BOLL)
		{
			enemy_sprite[i] = sprEnemyBall;
			enemy_setting(i, 40, 40, 20, 20, 2, 2, 0);
		}
		else if (enemy[i].type == ENEMY_TYPE_BALLOON)
		{
			enemy_sprite[i] = sprEnemyCrayon;
			enemy_setting(i, 64, 160, 32, 80, 1, 1, 0);
		}
		else if (enemy[i].type == ENEMY_TYPE_ROCKET)
		{
			enemy_sprite[i] = sprEnemyRocket;
			enemy_setting(i, 160, 40, 80, 20, 1, 1, 0);
		}
		else 
		{
			enemy_sprite[i] = sprEnemyCrayon;
			enemy_setting(i, 64, 160, 32, 80, 1, 1, 0);
			enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		}

		//出現場所
		if (enemy[i].type == ENEMY_TYPE_BALLOON)
		{
			enemy[i].pos.x = rand() % 400 + 1300;
			enemy[i].pos.y = -50.0f;
			enemy_target_y[i] = (float)(rand() % 400 + 600);
		}
		else if (enemy[i].type == ENEMY_TYPE_ROCKET)
		{
			enemy[i].pos.x = SCREEN_W + 50.0f;
			enemy[i].pos.y = rand() % 200 + 50;
			enemy_target_y[i] = (float)(rand() % 400 + 600);
		}
		else
		{
			enemy[i].pos.x = SCREEN_W + 100.0f;
			enemy[i].pos.y = rand() % 724 + 340;
		}

		enemy_prev_hp[i] = enemy[i].hp;
	}
}

//ボスのリスポーン
void enemy_boss_respawn(int i)
{
	enemy_type_render[i] = BOSS;
	enemy_sprite[i] = sprEnemyCrayon;
	enemy[i].color = { 1, 1, 1, 1 };
	if (fever_blue == false)
	{
		switch (boss_state)
		{
		case 0:
			enemy[i].type = BOSS_TYPE_BEAR;
			enemy_sprite[i] = sprEnemyBear;
			enemy_setting(i, 514, 560, 257, 280, 1, 1, 0);
			boss_state++;
			break;
		}

		//リスポーン
		if (enemy[i].type == BOSS_TYPE_BEAR)
		{
			enemy[i].pos.x = SCREEN_W - 200.0f;
			enemy[i].pos.y = -300.0f;
		}
		else
		{
			enemy[i].pos.x = SCREEN_W + 200.0f;
			enemy[i].pos.y = SCREEN_H * 0.5f;
		}

		enemy_prev_hp[i] = enemy[i].hp;
	}
}

//敵の進み方
void enemy_move(int i)
{
	enemy_border[i] = 270 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;
	if (fever_blue == false)
	{
		if (enemy[i].type == ENEMY_TYPE_BOLL ||
			enemy[i].type == ENEMY_TYPE_BOBM ||
			enemy[i].type == ENEMY_TYPE_ROCKET ||
			enemy[i].type == BOSS_TYPE_CRAYON ||
			enemy[i].type == BOSS_TYPE_HORSE ||
			enemy[i].type == BOSS_TYPE_PENCIL ||
			enemy[i].type == ENEMY_TYPE_CRAYON)
		{
			enemy[i].pos.x -= enemy[i].moveSpeed;
		}
		else if (enemy[i].type == ENEMY_TYPE_ROCKET)
		{
			enemy_border[i] = 300;
			enemy[i].pos.x -= enemy[i].moveSpeed;
		}
		else if (enemy[i].type == BOSS_TYPE_CAR)
		{
			enemy[i].pos.x -= enemy[i].moveSpeed;
			if (enemy[i].pos.x <= 1600 && enemy[i].pos.x >= 1400)
			{
				enemy[i].pos.x -= enemy[i].moveSpeed * 3;
				enemy[i].pos.y += enemy[i].moveSpeed * 8;
			}
			else if (enemy[i].pos.x <= 1200 && enemy[i].pos.x >= 1000)
			{
				enemy[i].pos.x -= enemy[i].moveSpeed * 3;
				enemy[i].pos.y -= enemy[i].moveSpeed * 8;
			}
			else if (enemy[i].pos.x <= 800 && enemy[i].pos.x >= 600)
			{
				enemy[i].pos.x -= enemy[i].moveSpeed * 3;
				enemy[i].pos.y += enemy[i].moveSpeed * 8;
			}
			else
				enemy[i].pos.x -= enemy[i].moveSpeed;
		}
		else if (enemy[i].type == ENEMY_TYPE_BALLOON)
		{
			if (enemy[i].pos.y < enemy_target_y[i])
			{
				swing_speed = cosf(enemy[i].pos.y * 0.05f) * 1.5f;
				enemy[i].pos.y += enemy[i].moveSpeed;
				enemy[i].pos.x += swing_speed;

				balloon_pos_x[i] = enemy[i].pos.x;
				balloon_pos_y[i] = enemy[i].pos.y - 130;
			}
			else
			{
				enemy[i].pos.x -= enemy[i].moveSpeed;
				balloon_pos_y[i] -= 5;
			}
		}
		else if (enemy[i].type == BOSS_TYPE_BEAR)
		{
			//待機中は着地の潰れを少しずつ元のスケールへ戻す
			enemy[i].scale.x += (1.0f - enemy[i].scale.x) * 0.18f;
			enemy[i].scale.y += (1.0f - enemy[i].scale.y) * 0.18f;

			//画面上から降下
			if (bear_dropping)
			{
				bear_vy += bear_drop_gravity;
				enemy[i].pos.y += bear_vy;

				//地面に到達したら潰れ＋待機へ
				if (enemy[i].pos.y >= bear_ground_y)
				{
					enemy[i].pos.y = bear_ground_y;
					bear_vy = 0.0f;
					bear_dropping = false;
					bear_land_wait = bear_land_wait_max;

					//重い着地の潰れ
					enemy[i].scale = { 1.22f, 0.78f };
				}
			}
			//着地後の待機
			else if (bear_land_wait > 0)
			{
				bear_land_wait--;

				//待機が終わる瞬間ジャンプ
				if (bear_land_wait == 0)
				{
					bear_vy = bear_jump_power;
					enemy[i].scale = { 0.92f, 1.10f }; //跳び上がるときに少し縦伸び
				}
			}
			//跳ねながら左へ進む
			else
			{
				enemy[i].pos.x -= enemy[i].moveSpeed;

				bear_vy += bear_jump_gravity;
				enemy[i].pos.y += bear_vy;

				//着地したら再び潰れて待機
				if (enemy[i].pos.y >= bear_ground_y)
				{
					enemy[i].pos.y = bear_ground_y;
					bear_vy = 0.0f;
					bear_land_wait = bear_land_wait_max;

					enemy[i].scale = { 1.18f, 0.82f };
				}
			}
		}
		if (enemy[i].type == ENEMY_TYPE_BOBM)
		{
			if (enemy[i].pos.x <= enemy_border[i])
			{
				player_hp -= (enemy[i].attack * 3);
				shake(50, 50);
				enemy_bomb_explosion(i);
				enemy[i].hp = 0;
			}
		}
	}
}

//敵の攻撃
void enemy_attack_action(int i)
{
	enemy_attack_timer[i]++;

	if (enemy_attack_timer[i] < 15)
		enemy[i].pos.x -= 3.0f;
	else if (enemy_attack_timer[i] == 15)
	{
		player_hp -= enemy[i].attack;
		shake(30, 30);
	}
	else if (enemy_attack_timer[i] > 15 && enemy_attack_timer[i] < 45)
		enemy[i].pos.x += 1.5f;
	else if (enemy_attack_timer[i] > 105)
	{
		enemy_attack_timer[i] = 0;
	}
}

//パワーアップ
void enemy_powerup()
{
	if (enemy_powerup_timer > 3000)
	{
		enemy_hp_up += 2.0f;
		enemy_speed_up += 0.2f;
		enemy_attack_up += 1;
		enemy_powerup_timer = 0;
	}
}

//ノックバック
void enemy_knockback_check(int i)
{
	if (enemy[i].hp <= enemy_max_hp[i] / 2 && !enemy_half_knockback_done[i])
	{
		enemy_knockback_timer[i] = 15;
		enemy_half_knockback_done[i] = true;
	}

	//タイマーがあるときは後ろに下がる
	if (enemy_knockback_timer[i] > 0)
	{
		enemy[i].pos.x += 3.0f + hit_buck;

		if (enemy_knockback_timer[i] > 7)
			enemy[i].pos.y -= 1.5f;
		else
			enemy[i].pos.y += 1.5f;

		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		enemy_knockback_timer[i]--;
	}
}

//ボムが爆発したときの周辺へのダメージ
void enemy_bomb_explosion(int i)
{
	particles_update(enemy[i].pos.x, enemy[i].pos.y, 8, 1.0f, 0.4f, 0.1f);

	for (int j = 0; j < ENEMY_NUMBER; j++)
	{
		if (i == j) continue;
		if (enemy[j].hp <= 0) continue;

		float enemy_Vec_x = enemy[j].pos.x - enemy[i].pos.x;
		float enemy_Vec_y = enemy[j].pos.y - enemy[i].pos.y;
		float enemy_Vec = sqrtf(enemy_Vec_x * enemy_Vec_x + enemy_Vec_y * enemy_Vec_y);

		if (enemy_Vec < 250.0f)
		{
			enemy[j].hp -= 15;
		}
	}
}

//CARのタイヤダメージ
void tires_move(float boss_x, float boss_y)
{
	// 4つのタイヤをすべて「右方向」へ勢いよく飛ばす
	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++)
	{
		tire_particles[i].active = true;

		//カーの中心付近からスポーン
		tire_particles[i].x = boss_x;
		tire_particles[i].y = boss_y;

		float speed = (float)(rand() % 5 + 12);

		switch (i)
		{
		case 0: //右斜め上へ（空高く右へぶっ飛ぶ）
			tire_particles[i].vx = speed * 1.2f;
			tire_particles[i].vy = -8.0f; // ぐっと上へ突き上げる
			tire_particles[i].sprite = sprTirefront;
			break;
		case 1: //まっすぐ右へ（水平に弾丸のように飛ぶ）
			tire_particles[i].vx = speed * 1.4f;
			tire_particles[i].vy = -3.0f;
			tire_particles[i].sprite = sprTireback;
			break;
		case 2: //やや低い軌道で右へ
			tire_particles[i].vx = speed * 1.3f;
			tire_particles[i].vy = -1.0f;
			tire_particles[i].sprite = sprTirefront;
			break;
		case 3: //右斜め下へ（地面を這うように猛スピードで右へ）
			tire_particles[i].vx = speed * 1.5f;
			tire_particles[i].vy = 2.0f;
			tire_particles[i].sprite = sprTireback;
			break;
		}

		tire_particles[i].rot = (float)(rand() % 360);
		tire_particles[i].rot_speed = (float)(rand() % 40 - 20) * 0.1f; // ぐるぐる高速回転
	}
}

void effect_boss_respawn()
{
	effect_boss_clear = 1.0f;

	if (etimer >= 0.0f)
	{
		effect_boss_wave = (sinf(etimer * 0.15f) + 1.0f) * 0.5f;
		effect_boss_clear = 0.3f + (0.7f * effect_boss_wave);
	}
	else
	{
		float fade_alpha = etimer / -30.0f;
		if (fade_alpha > 1.0f)
			fade_alpha = 1.0f;
		effect_boss_clear = 1.0f - fade_alpha;
		if (effect_boss_clear < 0.0f)
			effect_boss_clear = 0.0f;
	}

	//赤みの濃さに反映(最大値は0.8f)
	effect_boss_clear = 0.8f * effect_boss_clear;

	//警告画像の横スクロール
	x_1 -= 5;
	x_2 -= 5;
	if (x_1 < -1915)
		x_1 = 1915;
	if (x_2 < -1915)
		x_2 = 1915;

	etimer--;

	if (etimer < -30.0f)
	{
		respawn = false;
		x_1 = 0;
		x_2 = 1915;
		etimer = 300;
		effect_boss_clear = 0.0f;
	}
}

//ダメージを受けたときのエフェクト
void enemy_damage_effect(int i)
{
	if (enemy_damage_timer[i] > 0)
	{
		//被弾タイマーが残っている間は赤く光らせる
		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		enemy_damage_timer[i]--;
	}
	else
	{
		//わかりやすくするために赤くする
		if (enemy[i].type == ENEMY_TYPE_BOBM)
		{
			enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		}
		else
		{
			enemy[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		}
	}
}

void tires_render()
{
	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++)
	{
		if (!tire_particles[i].active) continue;

		//移動と重力の影響
		tire_particles[i].x += tire_particles[i].vx;
		tire_particles[i].y += tire_particles[i].vy;
		tire_particles[i].vy += 0.4f; //重力で少し下に落ちる

		//回転させる
		tire_particles[i].rot += tire_particles[i].rot_speed;

		//画面外（下や右）に行ったら非アクティブにする
		if (tire_particles[i].y > SCREEN_H + 100.0f)
		{
			tire_particles[i].active = false;
		}

		//描画
		sprite_render(
			tire_particles[i].sprite,
			tire_particles[i].x, tire_particles[i].y,
			1.5f, 1.5f,
			0, 0,
			40, 40,
			20, 20,
			tire_particles[i].rot,
			1.0f, 1.0f, 1.0f, 1.0f
		);
	}
}

void effect_boss_render()
{
	float fade_alpha = 0.0f;
	if (etimer < 0.0f)
	{
		fade_alpha = etimer / -30.0f;
		if (fade_alpha > 1.0f) fade_alpha = 1.0f;
	}
	float draw_alpha = 1.0f - fade_alpha;

	//背景の黒い暗転（だんだん薄くなる）
	primitive::rect(
		0, 0,
		1920, 1080,
		0, 0,
		0,
		0, 0, 0, 0.5f * draw_alpha);

	//赤い警告
	primitive::rect(
		0.0f, 0.0f,
		1920.0f, 1080.0f,
		0.0f, 0.0f,
		0.0f,
		1.0f, 0.0f, 0.0f, effect_boss_clear * draw_alpha);

	sprite_render(
		sprWarning,
		x_1, 0,
		1.0f, 1.0f,
		0, 0,
		1915, 1080,
		0, 0,
		0.0f,
		1.0f, 1.0f, 1.0f, draw_alpha);

	sprite_render(
		sprWarning,
		x_2, 0,
		1.0f, 1.0f,
		0, 0,
		1915, 1080,
		0, 0,
		0.0f,
		1.0f, 1.0f, 1.0f, draw_alpha);
}

//描画処理
void enemy_render()
{
	//死亡エフェクト
	particle_render();

	//CAR死亡エフェクト
	tires_render();

	//通常敵の描画
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		if (enemy[i].hp <= 0) { continue; }
		if (enemy_type_render[i] == BOSS) { continue; }

		float render_y = enemy[i].pos.y;
		float render_scale_x = enemy[i].scale.x;
		float render_scale_y = enemy[i].scale.y;

		if (fever_blue == false)
		{
			//クレヨンジャンプ
			if (enemy[i].type == ENEMY_TYPE_CRAYON)
			{
				float seed = (float)(i * 53);
				float time = (float)enemy_anim_timer[i] * 0.08f + seed;
				float s = sinf(time);
				if (s > 0.7f)
				{
					float jump_height = (s - 0.7f) * 30.0f;
					render_y -= jump_height;
				}
			}
		}

		//敵画像
		sprite_render(
			enemy_sprite[i],
			enemy[i].pos.x, render_y,
			render_scale_x, render_scale_y,
			0, 0,
			enemy[i].texSize.x, enemy[i].texSize.y,
			enemy[i].pivot.x, enemy[i].pivot.y,
			enemy[i].angle,
			enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);

		//ボールの影
		if (enemy[i].type == ENEMY_TYPE_BOLL)
		{
			sprite_render(
				sprShadow,
				enemy[i].pos.x + 5, render_y + 7,
				render_scale_x, render_scale_y,
				0, 0,
				enemy[i].texSize.x, enemy[i].texSize.y,
				enemy[i].pivot.x, enemy[i].pivot.y,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);
		}
		//風船
		if (enemy[i].type == ENEMY_TYPE_BALLOON)
		{
			sprite_render(
				sprBalloon,
				balloon_pos_x[i], balloon_pos_y[i],
				render_scale_x, render_scale_y,
				0, 0,
				80, 122,
				40, 61,
				ToRadian(0),
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);
		}

		//HP割合
		enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
		if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
		if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;

		//通常敵HPゲージ
		float bar_pos_y;
		if (enemy[i].type == ENEMY_TYPE_ROCKET)
		{
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];
			bar_pos_y = render_y - 50.0f;

			primitive::rect(enemy[i].pos.x - 25, bar_pos_y, 80, 10, 0, 0, 0, 1, 0, 0);
			primitive::rect(enemy[i].pos.x - 25, bar_pos_y, enemy_hp_bar_max[i], 10, 0, 0, 0, 0, 1, 0);

			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 28, bar_pos_y - 3);
		}
		else
		{
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];
			bar_pos_y = render_y - 100.0f;

			primitive::rect(enemy[i].pos.x - 40, bar_pos_y, 80, 10, 0, 0, 0, 1, 0, 0);
			primitive::rect(enemy[i].pos.x - 40, bar_pos_y, enemy_hp_bar_max[i], 10, 0, 0, 0, 0, 1, 0);

			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 43, bar_pos_y - 3);
		}
	}

	//ボス敵
	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		if (enemy[i].hp <= 0) { continue; }
		if (enemy_type_render[i] != BOSS) { continue; }

		float render_y = enemy[i].pos.y;
		float render_scale_x = enemy[i].scale.x;
		float render_scale_y = enemy[i].scale.y;

		sprite_render(
			enemy_sprite[i],
			enemy[i].pos.x, render_y,
			render_scale_x, render_scale_y,
			0, 0,
			enemy[i].texSize.x, enemy[i].texSize.y,
			enemy[i].pivot.x, enemy[i].pivot.y,
			enemy[i].angle,
			enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);

		//タイヤ
		if (enemy[i].type == BOSS_TYPE_CAR)
		{
			sprite_render(
				sprTirefront,
				enemy[i].pos.x - 30, render_y + 70,
				1.5, 1.5,
				0, 0,
				40, 40,
				20, 20,
				tire_angle[i],
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);

			sprite_render(
				sprTireback,
				enemy[i].pos.x + 80, render_y + 70,
				1.5, 1.5,
				0, 0,
				40, 40,
				20, 20,
				tire_angle[i],
				enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);
		}

		// HP割合
		enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
		if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
		if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;

		// ボスHPゲージ
		enemy_hp_bar_max[i] = 200.0f * enemy_hp_rate[i];

		float bar_width = 200.0f;
		float bar_x = enemy[i].pos.x - (bar_width * 0.5f);
		float bar_y = enemy[i].pos.y - (enemy[i].texSize.y * enemy[i].scale.y * 0.5f) + 30.0f;

		primitive::rect(bar_x, bar_y, bar_width, 20, 0, 0, 0, 1, 0, 0);
		primitive::rect(bar_x, bar_y, enemy_hp_bar_max[i], 20, 0, 0, 0, 0, 1, 0);

		sprite_render(
			sprBoss_hp,
			bar_x - 3, bar_y - 3);
	}

	//警告演出
	if (respawn == true)
	{
		effect_boss_render();
	}
}