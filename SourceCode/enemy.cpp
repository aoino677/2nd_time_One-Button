#include "player.h"
#include "enemy.h"
#include "shake.h"
#include "Hp.h"
#include "bullet.h"
#include "AttackCollision.h"
#include <vector>
#include <cstdlib>
#include <cmath> // sinf 用

#define PARTICLE_NUMBER 150 // 画面全体で同時に扱える破片の最大数

struct PARTICLE {
	bool active;      // 使用中かどうか
	float x, y;       // 座標
	float vx, vy;     // 速度（移動方向とスピード）
	float size;       // ブロックの大きさ
	float alpha;      // 透明度
	int life;         // 残り寿命
	int max_life;     // 全体の寿命
	float r, g, b;    // 色（赤・緑・青）
};

PARTICLE enemy_particle[PARTICLE_NUMBER];

Sprite* enemy_sprite[ENEMY_NUMBER];
Sprite* sprCrayon;                      // クレヨン画像
Sprite* sprBall;                        // ボール画像
Sprite* sprEnemy_hp;
Sprite* sprBoss_hp;

// すべての敵を共通の構造体配列で一元管理
OBJ2D enemy[ENEMY_NUMBER];

// state類
int enemy_state;
int boss_state;

// タイマー類
int enemy_powerup_timer;               // 強化タイマー
int enemy_boss_timer;                  // ボスタイマー
int crayon_spawn_timer;                // クレヨンタイマー
int enemy_attack_timer[ENEMY_NUMBER];  // 攻撃タイマー
int enemy_anim_timer[ENEMY_NUMBER];    // アニメーションタイマー

// 被弾・ノックバック用変数
int enemy_damage_timer[ENEMY_NUMBER];         // 通常被弾時の赤色点滅用タイマー
int enemy_knockback_timer[ENEMY_NUMBER];      // HP半分以下のノックバック用タイマー
bool enemy_half_knockback_done[ENEMY_NUMBER]; // HP半分以下のノックバックが完了したか
int enemy_prev_hp[ENEMY_NUMBER];              // ダメージ検知用の前回HP記憶配列

// 強化類
float enemy_hp_up = 0.0f;              // HP増加量
float enemy_speed_up = 0.0f;           // スピード増加量
float enemy_attack_up = 0.0f;          // 攻撃増加量

// HPゲージ類
int enemy_max_hp[ENEMY_NUMBER];        // HP最大量
float enemy_hp_bar_max[ENEMY_NUMBER];  // 最大HPゲージ
float enemy_hp_rate[ENEMY_NUMBER];     // HP割合

// エネミー情報
int enemy_max;                         // 最大リスポーン数
int enemy_type_render[ENEMY_NUMBER];   // 敵タイプ描画
float enemy_border[ENEMY_NUMBER];      // 最大移動値
float enemy_target_y[ENEMY_NUMBER];

// エネミー初期化
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
	}

	for (int i = 0; i < PARTICLE_NUMBER; i++) {
		enemy_particle[i].active = false;
	}
}

void enemy_deinit()
{
	safe_delete(sprCrayon);
	safe_delete(sprBall);
	safe_delete(sprEnemy_hp);
	safe_delete(sprBoss_hp);
}

void enemy_update()
{
	switch (enemy_state)
	{
	case 0:
		sprCrayon = sprite_load(L"Data/Images/crayon.png");
		sprBall = sprite_load(L"Data/Images/ball.png");
		sprEnemy_hp = sprite_load(L"Data/Images/enemy_hp.png");
		sprBoss_hp = sprite_load(L"Data/Images/boss_hp.png");

		enemy_state++;
		break;

	case 1:
		for (int i = 0; i < enemy_max; i++)
		{
			enemy[i].scale = { ENEMY_SCALE, ENEMY_SCALE };
			enemy[i].texSize = { ENEMY_TEX_W, ENEMY_TEX_H };
			enemy[i].color = { 1, 1, 1, 1 };

			enemy_ability(i);
		}

		spawn_crayon_formation();

		enemy_state++;
		break;

	case 2:
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			// 1. 生きている状態からHPが0以下になった瞬間（死亡＆パーティクル発生）
			if (enemy[i].hp <= 0 && enemy_prev_hp[i] > 0)
			{
				if (enemy_type_render[i] == BOSS)
				{
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.6f, 0.1f);
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 1.0f, 1.0f);
				}
				else if (enemy[i].type == ENEMY_TYPE_BOBM)
				{
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 1.0f, 0.2f, 0.2f);
				}
				else
				{
					spawn_enemy_particles(enemy[i].pos.x, enemy[i].pos.y, 0.9f, 0.9f, 0.9f);
				}
			}

			// 2. ダメージを受けた瞬間の判定（前回のHPより減っていれば被弾タイマーセット）
			if (enemy[i].hp < enemy_prev_hp[i] && enemy[i].hp > 0)
			{
				enemy_damage_timer[i] = 10; // 約0.16秒間赤く光る（60fps基準）
			}

			// 3. 現在のHPを次フレームの比較用に保存
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
					enemy_ability(i);
				}
				continue;
			}

			if (enemy[i].pos.x > enemy_border[i] || enemy[i].pos.x > 530)
			{
				enemy_move(i);
			}
			else
			{
				if (enemy[i].type != ENEMY_TYPE_BOBM)
				{
					enemy_attack_action(i);
				}
			}

			// ダメージ時の赤色点滅カラー変更処理を呼び出し
			enemy_damage_flash(i);

			enemy_knockback_check(i);

			enemy_anim_timer[i]++;
		}

		enemy_powerup_timer++;
		enemy_boss_timer++;
		crayon_spawn_timer++;

		if (enemy_boss_timer >= 1800)
		{
			for (int i = enemy_max; i < ENEMY_NUMBER; i++)
			{
				if (enemy[i].hp <= 0)
				{
					enemy_boss_respawn(i);
					enemy_boss_timer = 0;
					break;
				}
			}
		}

		if (crayon_spawn_timer >= 600)
		{
			spawn_crayon_formation();
			crayon_spawn_timer = 0;
		}

		enemy_powerup();

		break;
	}
}

// ボムが爆発したときの周辺へのダメージ処理
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

		if (enemy_Vec < 300.0f)
		{
			enemy[j].hp -= 15;
		}
	}
}

// ダメージを受けたときのカラーフラッシュ処理
void enemy_damage_flash(int i)
{
	if (enemy_damage_timer[i] > 0)
	{
		// 被弾タイマーが残っている間は赤く光らせる
		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
		enemy_damage_timer[i]--;
	}
	else
	{
		// ボム敵の場合は普段から赤みを帯びた色を維持、それ以外は通常の色（白）に戻す
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

void particle_update_and_render()
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

void enemy_knockback_check(int i)
{
	if (enemy[i].hp <= enemy_max_hp[i] / 2 && !enemy_half_knockback_done[i])
	{
		enemy_knockback_timer[i] = 15;
		enemy_half_knockback_done[i] = true;
	}

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

void enemy_ability(int i)
{
	enemy_type_render[i] = ENEMY;

	int enemy_random = rand() % 60;

	if (enemy_random < 20)
	{
		enemy[i].type = ENEMY_TYPE_BOLL;
		enemy_sprite[i] = sprBall;
		enemy_setting(i, 3, 1, 3, 20, 40, 1, 1);
		enemy[i].scale = { 1,1 };
	}
	else if (enemy_random >= 20 && enemy_random < 40)
	{
		enemy[i].type = ENEMY_TYPE_BALLOON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 3, 20, 40, 1, 1);
		enemy[i].scale = { 1,1 };
	}
	else if (enemy_random >= 40)
	{
		enemy[i].type = ENEMY_TYPE_BOBM;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 3, 1, 3, 20, 40, 1, 1);
		enemy[i].scale = { 1,1 };
		enemy[i].color = { 1.0f, 0.3f, 0.3f, 1.0f };
	}

	if (enemy[i].type == ENEMY_TYPE_BALLOON)
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

	enemy_prev_hp[i] = enemy[i].hp;
}

void spawn_crayon_formation()
{
	int squad_max = 7;
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
				enemy_setting(idx, 3, 3, 3, 18, 40, 1, 1);
				enemy[idx].color = { 1.0f, 0.3f, 0.3f, 1.0f };
			}
			else
			{
				enemy[idx].type = ENEMY_TYPE_CRAYON_SQUAD;
				enemy_sprite[idx] = sprCrayon;
				enemy_setting(idx, 20, 2, 3, 18, 40, 1, 1);
				enemy[idx].color = { 1.0f, 1.0f, 1.0f, 1.0f };
			}

			enemy_type_render[idx] = ENEMY;
			enemy[idx].scale = { 1, 1 };
			enemy[idx].texSize = { ENEMY_TEX_W, ENEMY_TEX_H };

			enemy[idx].pos.x = SCREEN_W + 50.0f - (spawned_count * 20.0f);
			enemy[idx].pos.y = 420.0f + (spawned_count * 100.0f);

			enemy_prev_hp[idx] = enemy[idx].hp;
			spawned_count++;

			if (spawned_count >= squad_max)
			{
				break;
			}
		}
	}
}

void enemy_boss_respawn(int i)
{
	enemy_type_render[i] = BOSS;
	enemy_sprite[i] = sprCrayon;
	enemy[i].scale = { 3, 3 };
	enemy[i].texSize = { ENEMY_TEX_W, ENEMY_TEX_H };
	enemy[i].color = { 1, 1, 1, 1 };

	switch (boss_state)
	{
	case 0:
		enemy[i].type = BOSS_TYPE_CRAYON;
		enemy_sprite[i] = sprCrayon;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 1:
		enemy[i].type = BOSS_TYPE_MINICAR;
		enemy_sprite[i] = sprBall;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 2:
		enemy[i].type = BOSS_TYPE_DOLL;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 3:
		enemy[i].type = BOSS_TYPE_ROBOT;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 4:
		enemy[i].type = BOSS_TYPE_HORSE;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 5:
		enemy[i].type = BOSS_TYPE_PENCIL;
		enemy_setting(i, 100, 1, 2, 18, 40, 3, 3);
		boss_state++;
		break;
	case 6:
		enemy[i].type = BOSS_TYPE_EXCAVATOR;
		enemy_setting(i, 1000, 1, 2, 18, 40, 3, 3);
		break;
	}

	enemy[i].pos.x = SCREEN_W + 200.0f;
	enemy[i].pos.y = SCREEN_H * 0.5f;

	enemy_prev_hp[i] = enemy[i].hp;
}

void enemy_move(int i)
{
	enemy_border[i] = 270 + (SCREEN_H - enemy[i].pos.y) * 0.3584f / 0.9336f;

	if (enemy[i].type == ENEMY_TYPE_BOLL ||
		enemy[i].type == ENEMY_TYPE_BOBM ||
		enemy[i].type == BOSS_TYPE_CRAYON ||
		enemy[i].type == BOSS_TYPE_MINICAR ||
		enemy[i].type == BOSS_TYPE_DOLL ||
		enemy[i].type == BOSS_TYPE_ROBOT ||
		enemy[i].type == BOSS_TYPE_HORSE ||
		enemy[i].type == BOSS_TYPE_PENCIL ||
		enemy[i].type == BOSS_TYPE_EXCAVATOR ||
		enemy[i].type == ENEMY_TYPE_CRAYON_SQUAD)
	{
		enemy[i].pos.x -= enemy[i].moveSpeed;
	}
	else if (enemy[i].type == ENEMY_TYPE_BALLOON)
	{
		if (enemy[i].pos.y < enemy_target_y[i])
			enemy[i].pos.y += enemy[i].moveSpeed * 2;
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

void enemy_setting(int i, int hp, int attack, float speed, float x, float y, float a, float b)
{
	enemy_max_hp[i] = hp + (int)enemy_hp_up;
	enemy[i].hp = enemy_max_hp[i];
	enemy_prev_hp[i] = enemy[i].hp;

	enemy[i].attack = attack + (int)enemy_attack_up;
	enemy_damage_timer[i] = 0;

	enemy[i].moveSpeed = speed + enemy_speed_up;
	enemy[i].pivot = { x,y };
	enemy[i].scale = { a,b };

	enemy_knockback_timer[i] = 0;
	enemy_half_knockback_done[i] = false;
}

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

void enemy_powerup()
{
	if (enemy_powerup_timer > 1200)
	{
		enemy_hp_up += 10.0f;
		enemy_speed_up += 0.5f;
		enemy_powerup_timer = 0;
	}
}

//描画処理
void enemy_render()
{
	particle_update_and_render();

	for (int i = 0; i < ENEMY_NUMBER; i++)
	{
		if (enemy[i].hp <= 0) { continue; }

		float render_y = enemy[i].pos.y;
		float render_scale_x = enemy[i].scale.x;
		float render_scale_y = enemy[i].scale.y;

		if (enemy[i].type == ENEMY_TYPE_CRAYON_SQUAD)
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

		sprite_render(
			enemy_sprite[i],
			enemy[i].pos.x, render_y,
			render_scale_x, render_scale_y,
			0, 0,
			enemy[i].texSize.x, enemy[i].texSize.y,
			enemy[i].pivot.x, enemy[i].pivot.y,
			ToRadian(0),
			enemy[i].color.x, enemy[i].color.y, enemy[i].color.z);

		enemy_hp_rate[i] = (float)enemy[i].hp / (float)enemy_max_hp[i];
		if (enemy_hp_rate[i] < 0.0f) enemy_hp_rate[i] = 0.0f;
		if (enemy_hp_rate[i] > 1.0f) enemy_hp_rate[i] = 1.0f;

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
			enemy_hp_bar_max[i] = 80.0f * enemy_hp_rate[i];
			float bar_pos_y = render_y - 60.0f;

			primitive::rect(enemy[i].pos.x - 40, bar_pos_y, 80, 10, 0, 0, 0, 1, 0, 0);
			primitive::rect(enemy[i].pos.x - 40, bar_pos_y, enemy_hp_bar_max[i], 10, 0, 0, 0, 0, 1, 0);

			sprite_render(
				sprEnemy_hp,
				enemy[i].pos.x - 43, bar_pos_y - 3);
		}
	}

	debug::setString("enemy_max:%d", enemy_max);
}