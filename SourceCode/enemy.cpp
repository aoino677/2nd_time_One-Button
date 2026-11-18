#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include "bullet.h"
#include "AttackCollision.h"
#include "item.h"
#include <vector>
#include <cstdlib>
#include <cmath>

//enemy構造体
OBJ2D enemy[ENEMY_NUMBER];

PARTICLE enemy_particle[PARTICLE_NUMBER];

TireEffect tire_particles[TIRE_PARTICLE_NUMBER];

//敵画像
Sprite* enemy_sprite[ENEMY_NUMBER];
Sprite* sprCrayon;                      //クレヨン画像
Sprite* sprBall;                        //ボール画像
Sprite* sprRocket;                      //ロケット画像
Sprite* sprCar;                         //カー画像

//敵サブ画像
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

//強化類
float enemy_hp_up;               //HP増加量
float enemy_speed_up;            //スピード増加量
float enemy_attack_up ;          //攻撃増加量

//HPゲージ類
int enemy_max_hp[ENEMY_NUMBER];        //HP最大量
float enemy_hp_bar_max[ENEMY_NUMBER];  //最大HPゲージ
float enemy_hp_rate[ENEMY_NUMBER];     //HP割合

//エネミー情報
int enemy_max;                         // 最大リスポーン数
int enemy_type_render[ENEMY_NUMBER];   // 敵タイプ描画
float enemy_border[ENEMY_NUMBER];      // 最大移動値
float enemy_target_y[ENEMY_NUMBER];

float tire_angle[ENEMY_NUMBER];
float gravity=0.5;

//仮変数
bool respawn;
float x_1 = 0;
float x_2 = 1915;
float etimer=300;
float effect_boss_clear = 0.0f;
float effect_boss_timer = 0.0f;
float effect_boss_wave = 0.0f;
float swing_speed;
int robot_wait_timer = 0;


//エネミー初期化
void enemy_init()
{
	enemy_state = 0;
	boss_state = 0;
	enemy_powerup_timer = 0;
	enemy_boss_timer = 0;
	crayon_spawn_timer = 0;
	enemy_max = 3;


	enemy_hp_up = 0.0f;
	enemy_speed_up = 0.0f;
	enemy_attack_up = 0.0f;

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

	for (int i = 0; i < PARTICLE_NUMBER; i++) 
	{
		enemy_particle[i].active = false;
	}

	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++) {
		tire_particles[i].active = false;
	}
}

void enemy_deinit()
{
	safe_delete(sprCrayon);
	safe_delete(sprBall);
	safe_delete(sprShadow);
	safe_delete(sprRocket);
	safe_delete(sprCar);
	safe_delete(sprTirefront);
	safe_delete(sprTireback);
	safe_delete(sprEnemy_hp);
	safe_delete(sprBoss_hp);
	safe_delete(sprWarning);
}

void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		sprCrayon = sprite_load(L"Data/Images/crayon.png");
		sprBall = sprite_load(L"Data/Images/ball.png");
		sprRocket = sprite_load(L"Data/Images/rocket.png");
		sprShadow = sprite_load(L"Data/Images/ball_shadow.png");
		sprCar = sprite_load(L"Data/Images/car.png");
		sprTirefront = sprite_load(L"Data/Images/tire_front.png");
		sprTireback = sprite_load(L"Data/Images/tire_back.png");
		sprEnemy_hp = sprite_load(L"Data/Images/enemy_hp.png");
		sprBoss_hp = sprite_load(L"Data/Images/boss_hp.png");
		sprWarning = sprite_load(L"Data/Images/warning.png");

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
				item_drop(i);
				if (enemy_type_render[i] == BOSS)
				{
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.6f, 0.1f);
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 1.0f, 1.0f);
				
					if (enemy[i].type == BOSS_TYPE_CAR)
						tires_move(enemy[i].pos.x, enemy[i].pos.y);
					
				}
				else if (enemy[i].type == ENEMY_TYPE_BOBM)
				    spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.2f, 0.2f);
			    else if (enemy[i].type == ENEMY_TYPE_CRAYON)
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 0.9f, 0.9f, 0.9f);
				else
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.0f, 0.0f);
			}

			//ダメージを受けた瞬間の判定（前回のHPより減っていれば被弾タイマーセット）
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

			//防衛ライン
			if (enemy[i].pos.x > enemy_border[i] || enemy[i].pos.x > 530)
			{
				//回転
				if (enemy[i].type == ENEMY_TYPE_BOLL)
					enemy[i].angle -= 0.06;
				if (enemy[i].type == BOSS_TYPE_CAR)
					tire_angle[i] -= 0.06;
				
				enemy_move(i);
			}
			else
			{
				if (enemy[i].type != ENEMY_TYPE_BOBM)
				{
					enemy_attack_action(i);
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

		//ボスリスポーン
		if (enemy_boss_timer >=600)
		{
			for (int i = enemy_max; i < ENEMY_NUMBER; i++)
			{
				if (enemy[i].hp <= 0)
				{
					respawn = true;
					enemy_boss_respawn(i);
					enemy_boss_timer = 0;
					break;
				}
			}
		}
		if (respawn == true)
		{
			effect_boss_respawn();
			etimer--;
		}
		
		//クレヨンリスポーン
		if (crayon_spawn_timer >= 140)
		{
			enemy_crayon_respawn();
			crayon_spawn_timer = 0;
		}
		
		//時間経過でパワーアップ
		enemy_powerup();

		break;
	}
}

//敵の初期設定(番号・HP・攻撃力・スピード・画像幅・中心点・スケール・角度)
void enemy_setting(int i, int hp, int attack, float speed, float a, float b, float x, float y, float  n, float  m, int z)
{
	enemy_max_hp[i] = hp + (int)enemy_hp_up;
	enemy[i].hp = enemy_max_hp[i];
	enemy_prev_hp[i] = enemy[i].hp;

	enemy[i].attack = attack + (int)enemy_attack_up;
	enemy_damage_timer[i] = 0;

	enemy[i].moveSpeed = speed + enemy_speed_up;
	enemy[i].texSize = { a,b };
	enemy[i].pivot = { x,y };
	enemy[i].scale = { n,m };
	enemy[i].angle = z;

	enemy_knockback_timer[i] = 0;
	enemy_half_knockback_done[i] = false;
}

//クレヨンリスポーン
void enemy_crayon_respawn()
{
	int squad_max = 5;
	int spawned_count = 0;

	for (int i = enemy_max; i < ENEMY_NUMBER; i++)
	{
		if (enemy[i].hp <= 0)
		{
			int idx = i;
			bool is_bomb = (rand() % 100 < 15);

			if (is_bomb)
			{
				enemy[idx].type = ENEMY_TYPE_BOBM;
				enemy_sprite[idx] = sprCrayon;
				enemy_setting(i, 3, 1, 1, 64, 160, 32, 80, 1, 1, 0);
				enemy[idx].color = { 1.0f, 0.3f, 0.3f, 1.0f };
			}
			else
			{
				enemy[idx].type = ENEMY_TYPE_CRAYON;
				enemy_sprite[idx] = sprCrayon;
				enemy_setting(i, 3, 1, 1, 64, 160, 32, 80, 1, 1, 0);
				enemy[idx].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}

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

//敵設定
void enemy_respawn(int i)
{
	enemy_type_render[i] = ENEMY;

	int enemy_random = rand() % 25;

	if (enemy_random <= 25)
	{
		enemy[i].type = ENEMY_TYPE_BALLOON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 1, 1, 2, 64, 160, 32, 80, 1, 1, 0);
	}
	
	/*if (enemy_random < 25)
	{
		enemy[i].type = ENEMY_TYPE_BOLL;
		enemy_sprite[i] = sprBall;
		enemy_setting(i, 3, 1, 5, 40, 40, 20, 20, 2, 2,0);
	}
	else if (enemy_random >= 25 && enemy_random < 50)
	{
		enemy[i].type = ENEMY_TYPE_BALLOON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 1, 1, 2, 64, 160, 32, 80, 1, 1,0);
	}
	else if (enemy_random >= 50 && enemy_random < 75)
	{
		enemy[i].type = ENEMY_TYPE_ROCKET;
		enemy_sprite[i] = sprRocket;
		enemy_setting(i, 1, 1, 1, 160, 40, 80, 20, 1, 1,0);
	}
	else if (enemy_random >= 75)
	{
		enemy[i].type = ENEMY_TYPE_BOBM;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 1, 64, 160, 32, 80, 1, 1,0);
		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
	}*/

	//出現場所
	if (enemy[i].type == ENEMY_TYPE_BALLOON)
	{
		enemy[i].pos.x = rand() % 400 + 1300;
		enemy[i].pos.y = -50.0f;
		enemy_target_y[i] = (float)(rand() % 400 + 600);
	}
	else if (enemy[i].type == ENEMY_TYPE_ROCKET)
	{
		enemy[i].pos.x = enemy[i].pos.x = SCREEN_W + 50.0f;
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

//ボスのリスポーン
void enemy_boss_respawn(int i)
{
	enemy_type_render[i] = BOSS;
	enemy_sprite[i] = sprCrayon;
	enemy[i].color = { 1, 1, 1, 1 };

	switch (boss_state)
	{
	case 0:
		enemy[i].type = BOSS_TYPE_ROBOT;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 300, 1, 0.5, 64, 160, 32, 80, 3, 3, 0);
		//boss_state++;

	/*case 0:
		enemy[i].type = BOSS_TYPE_CRAYON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 300, 1, 0.5, 64, 160,32,80, 3, 3,0);
		boss_state++;
		break;
	case 1:
		enemy[i].type = BOSS_TYPE_CAR;
		enemy_sprite[i] = sprCar;
		enemy_setting(i, 100, 1, 0.5, 128, 80, 64, 40, 2.5, 2.5,0);
		boss_state++;
		break;
	case 2:
		enemy[i].type = BOSS_TYPE_DOLL;
		enemy_setting(i, 100, 1, 2, 64, 160, 32, 80, 3, 3,0);
		boss_state++;
		break;
	case 3:
		enemy[i].type = BOSS_TYPE_ROBOT;
		enemy_setting(i, 100, 1, 2, 64, 160, 32, 80, 3, 3,0);
		boss_state++;
		break;
	case 4:
		enemy[i].type = BOSS_TYPE_HORSE;
		enemy_setting(i, 100, 1, 2, 64, 160, 32, 80, 3, 3,0);
		boss_state++;
		break;
	case 5:
		enemy[i].type = BOSS_TYPE_PENCIL;
		enemy_setting(i, 100, 1, 2, 64, 160, 32, 80, 3, 3,0);
		boss_state++;
		break;
	case 6:
		enemy[i].type = BOSS_TYPE_EXCAVATOR;
		enemy_setting(i, 100, 1, 2, 64, 160, 32, 80, 3, 3,0);
		break;*/
	}

	//リスポーン
	if (enemy[i].type == BOSS_TYPE_ROBOT)
	{
		enemy[i].pos.x = SCREEN_W - 100.0f;
		enemy[i].pos.y = -300.0f;
	}
	else
	{
		enemy[i].pos.x = SCREEN_W + 200.0f;
		enemy[i].pos.y = SCREEN_H * 0.5f;
	}

	enemy_prev_hp[i] = enemy[i].hp;
}

//敵の進み方
void enemy_move(int i)
{
	enemy_border[i] = 270 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;

	if (enemy[i].type == ENEMY_TYPE_BOLL ||
		enemy[i].type == ENEMY_TYPE_BOBM ||
		enemy[i].type == ENEMY_TYPE_ROCKET ||
		enemy[i].type == BOSS_TYPE_CRAYON ||
		enemy[i].type == BOSS_TYPE_DOLL ||
		enemy[i].type == BOSS_TYPE_HORSE ||
		enemy[i].type == BOSS_TYPE_PENCIL ||
		enemy[i].type == ENEMY_TYPE_CRAYON)
	{
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
	else if (enemy[i].type == BOSS_TYPE_ROBOT)
	{

		if (enemy[i].pos.y < 500)
		{
			float drop_speed = 2.0f + (enemy_anim_timer[i] * 0.3f);
			enemy[i].pos.y += drop_speed;
			robot_wait_timer = 120;
		}
		else
		{
			//しばらく待機する
			if (robot_wait_timer > 0)
				robot_wait_timer--; 
			else
				enemy[i].pos.x -= enemy[i].moveSpeed;
		}
	}
	else if (enemy[i].type == ENEMY_TYPE_BALLOON)
	{
		if (enemy[i].pos.y < enemy_target_y[i])
		{
			swing_speed = cosf(enemy[i].pos.y * 0.05f) * 1.5f;
			enemy[i].pos.y += enemy[i].moveSpeed ;
			enemy[i].pos.x += swing_speed;
		}
		else
			enemy[i].pos.x -= enemy[i].moveSpeed;
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

//敵の攻撃
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
	{
		enemy_attack_timer[i] = 0;
	}
}

//パワーアップ
void enemy_powerup()
{
	if (enemy_powerup_timer > 1200)
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
		enemy[i].pos.x += 3.0f;

		if (enemy_knockback_timer[i] > 7)
		{
			enemy[i].pos.y -= 1.5f;
		}
		else
		{
			enemy[i].pos.y += 1.5f;
		}

		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		enemy_knockback_timer[i]--;
	}
}

//ボムが爆発したときの周辺へのダメージ
void enemy_bomb_explosion(int i)
{
	spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.4f, 0.1f);

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
		// ボスカーの中心付近からスポーンさせる
		tire_particles[i].x = boss_x;
		tire_particles[i].y = boss_y;

		// --- 右へぶっ飛ぶように初速（vx）をかなり大きく、重力をゼロ（またはマイナス）にする ---
		float speed = (float)(rand() % 5 + 12); // 速度 12 〜 16（かなり高速！）

		switch (i)
		{
		case 0: // 右斜め上へ（空高く右へぶっ飛ぶ）
			tire_particles[i].vx = speed * 1.2f;
			tire_particles[i].vy = -8.0f; // ぐっと上へ突き上げる
			tire_particles[i].sprite = sprTirefront;
			break;
		case 1: // まっすぐ右へ（水平に弾丸のように飛ぶ）
			tire_particles[i].vx = speed * 1.4f;
			tire_particles[i].vy = -3.0f;
			tire_particles[i].sprite = sprTireback;
			break;
		case 2: // やや低い軌道で右へ
			tire_particles[i].vx = speed * 1.3f;
			tire_particles[i].vy = -1.0f;
			tire_particles[i].sprite = sprTirefront;
			break;
		case 3: // 右斜め下へ（地面を這うように猛スピードで右へ）
			tire_particles[i].vx = speed * 1.5f;
			tire_particles[i].vy = 2.0f;
			tire_particles[i].sprite = sprTireback;
			break;
		}

		tire_particles[i].rot = (float)(rand() % 360);
		tire_particles[i].rot_speed = (float)(rand() % 40 - 20) * 0.1f; // ぐるぐる高速回転
	}
}

// 毎フレームのタイヤの動きと描画を更新する関数
void tires_update()
{
	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++)
	{
		if (!tire_particles[i].active) continue;

		// 移動と重力の影響
		tire_particles[i].x += tire_particles[i].vx;
		tire_particles[i].y += tire_particles[i].vy;
		tire_particles[i].vy += 0.1f; // 重力で少し下に落ちる

		// 回転させる
		tire_particles[i].rot += tire_particles[i].rot_speed;

		// 画面外（下や右）に行ったら非アクティブにする
		if (tire_particles[i].y > SCREEN_H + 100.0f)
		{
			tire_particles[i].active = false;
		}

		// 描画
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

void effect_boss_respawn()
{
	// 始まりはパッと出しつつ、etimer が 0 以上の間は赤みが「薄くなったり濃くなったり」周期的に変化する
	effect_boss_clear = 1.0f;

	if (etimer >= 0.0f)
	{
		// etimer の進みに合わせて sinf を動かすことで、赤みが明滅します
		// 周期の速さは掛け算する数値（例: 0.1f）で調整できます
		effect_boss_wave = (sinf(etimer * 0.15f) + 1.0f) * 0.5f; // 0.0 〜 1.0 の間で往復

		// 例として、真っ暗（0.3）〜真っ赤（1.0）の間でフワフワ明滅させる場合
		effect_boss_clear = 0.3f + (0.7f * effect_boss_wave);
	}
	else
	{
		// etimer が 0 を切ってからの終わり際は、だんだん薄くフェードアウトして消える
		float fade_alpha = etimer / -30.0f; // 0 から -30 にかけて進行
		if (fade_alpha > 1.0f) fade_alpha = 1.0f;
		effect_boss_clear = 1.0f - fade_alpha; // 1.0 → 0.0 へ
		if (effect_boss_clear < 0.0f) effect_boss_clear = 0.0f;
	}

	// 赤みの濃さに反映（最大値を 0.8f などに調整）
	effect_boss_clear = 0.8f * effect_boss_clear;

	// 警告画像の横スクロール
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

//死亡エフェクト
void spawn_enemy_particles(float x, float y, float r, float g, float b)
{
	int spawn_count = 12;
	int created = 0;

	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (!enemy_particle[i].active)
		{
			enemy_particle[i].active = true;
			enemy_particle[i].x = x;
			enemy_particle[i].y = y;

			float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
			float speed = (float)(rand() % 8 + 2);
			enemy_particle[i].vx = cosf(angle) * speed;
			enemy_particle[i].vy = sinf(angle) * speed;

			enemy_particle[i].size = (float)(rand() % 8 + 6);
			enemy_particle[i].max_life = rand() % 20 + 20;
			enemy_particle[i].life = enemy_particle[i].max_life;
			enemy_particle[i].alpha = 1.0f;

			enemy_particle[i].r = r;
			enemy_particle[i].g = g;
			enemy_particle[i].b = b;

			created++;
			if (created >= spawn_count) break;
		}
	}
}

//死亡エフェクト
void particle_render()
{
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (!enemy_particle[i].active) continue;

		enemy_particle[i].life--;
		if (enemy_particle[i].life <= 0)
		{
			enemy_particle[i].active = false;
			continue;
		}

		enemy_particle[i].x += enemy_particle[i].vx;
		enemy_particle[i].y += enemy_particle[i].vy;

		enemy_particle[i].vx *= 0.92f;
		enemy_particle[i].vy *= 0.92f;

		enemy_particle[i].alpha = (float)enemy_particle[i].life / (float)enemy_particle[i].max_life;

		primitive::rect(
			enemy_particle[i].x - enemy_particle[i].size * 0.5f,
			enemy_particle[i].y - enemy_particle[i].size * 0.5f,
			enemy_particle[i].size,
			enemy_particle[i].size,
			0,
			enemy_particle[i].r,
			enemy_particle[i].g,
			enemy_particle[i].b,
			enemy_particle[i].alpha,
			1
		);
	}
}

void tires_render()
{
	for (int i = 0; i < TIRE_PARTICLE_NUMBER; i++)
	{
		if (!tire_particles[i].active) continue;

		// 移動と重力の影響
		tire_particles[i].x += tire_particles[i].vx;
		tire_particles[i].y += tire_particles[i].vy;
		tire_particles[i].vy += 0.4f; // 重力で少し下に落ちる

		// 回転させる
		tire_particles[i].rot += tire_particles[i].rot_speed;

		// 画面外（下や右）に行ったら非アクティブにする
		if (tire_particles[i].y > SCREEN_H + 100.0f)
		{
			tire_particles[i].active = false;
		}

		// 描画
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
	
	
		// etimer が 0 を切ってからだんだん薄くする割合（0.0f（濃い） 〜 1.0f（完全に消える））を計算
		float fade_alpha = 0.0f;
		if (etimer < 0.0f)
		{
			// etimer が 0 から -30 にかけて、0.0 → 1.0 へ進む割合を作る
			fade_alpha = etimer / -30.0f;
			if (fade_alpha > 1.0f) fade_alpha = 1.0f;
		}
		// 描画用の「残存不透明度」（1.0 → 0.0 へ減っていく）
		float draw_alpha = 1.0f - fade_alpha;

		// 背景の黒い暗転（だんだん薄くなる）
		primitive::rect(
			0, 0,
			1920, 1080,
			0, 0,
			0,
			0, 0, 0, 0.5f * draw_alpha
		);

		// 赤い警告レイヤー（だんだん薄くなる）
		primitive::rect(
			0.0f, 0.0f,
			1920.0f, 1080.0f,
			0.0f, 0.0f,
			0.0f,
			1.0f, 0.0f, 0.0f, effect_boss_clear * draw_alpha
		);

		// ★ 警告画像も `draw_alpha` をかけてだんだん薄くする！
		sprite_render(
			sprWarning,
			x_1, 0,
			1.0f, 1.0f,
			0, 0,
			1915, 1080, // ※お手元の画像のテクスチャサイズに合わせてください
			0, 0,
			0.0f,
			1.0f, 1.0f, 1.0f, draw_alpha // 最後の引数が透明度（アルファ）です
		);
		sprite_render(
			sprWarning,
			x_2, 0,
			1.0f, 1.0f,
			0, 0,
			1915, 1080, // ※お手元の画像のテクスチャサイズに合わせてください
			0, 0,
			0.0f,
			1.0f, 1.0f, 1.0f, draw_alpha // 最後の引数が透明度（アルファ）です
		);
	
}

//描画処理
void enemy_render()
{
	//死亡エフェクト
	particle_render();

	//CAR死亡エフェクト
	tires_render();

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		if (enemy[i].hp <= 0) { continue; }

		float render_y = enemy[i].pos.y;
		float render_scale_x = enemy[i].scale.x;
		float render_scale_y = enemy[i].scale.y;

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


		//HP割合
		enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
		if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
		if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;

		//HPゲージの大きさ
		if (enemy_type_render[i] == BOSS)
		{
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
		else
		{
			float bar_pos_y;
			if (enemy[i].type == ENEMY_TYPE_ROCKET)
			{
				enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];
				bar_pos_y = render_y - 40.0f;

				primitive::rect(enemy[i].pos.x - 40, bar_pos_y, 80, 10, 0, 0, 0, 1, 0, 0);
				primitive::rect(enemy[i].pos.x - 40, bar_pos_y, enemy_hp_bar_max[i], 10, 0, 0, 0, 0, 1, 0);

				sprite_render(
					sprEnemy_hp,
					enemy[i].pos.x - 43, bar_pos_y - 3);
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

	}
		
	if (respawn == true)
	{
		effect_boss_render();
	}

	debug::setString("etimer:%f", etimer);
	
    
}