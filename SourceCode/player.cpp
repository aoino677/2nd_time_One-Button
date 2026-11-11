#include "player.h"
#include "Bullet.h"
#include "enemy.h"
#include "AttackCollision.h"
#include "shake.h"
#include <vector>
#include "Collision2D.h"

float bullet_speed = BULLET_SPEED;//発射する弾の1秒当たりの速度
float shot_per_second = 20.0f; //１秒間に発射する弾の数	
float rapid_coolDown = RAPID_COOLDOWN; //連射開始時間
float precision_rad = ToRadian(5); //狙った方向に飛ぶ精度

float shot_timer;			//弾発射の待機時間
float rapid_timer;			//完了したら連射に移行する
bool is_rapid;				//連射中か

bool can_rapid_shot;		//連射可能か
int rapidShoted_bullet_count;//連射した弾数
int cast_bullet;			//発射を待機している弾

int bullet_lay_count = 9;
float bullet_lay_gap = ToRadian(7);//弾の間隔
float bullet_scale = 1.0f;//弾の大きさ

Sprite* sprPlayer;
Sprite* sprBullet[2];
OBJ2D player;
std::vector<Bullet*> player_bullets;

VECTOR2 cursor_pos{};

void player_init()
{
	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet[0] = sprite_load(L"Data/Images/bullet.png");
	sprBullet[1] = sprite_load(L"Data/Images/1yen.png");

	player.pos = PLAYER_POS;
	player.texSize = PLAYER_TEX_SIZE;
	player.pivot = player.texSize / 2;
	bullet_scale = 1.0f;
	shot_timer = 0;
	rapidShoted_bullet_count = 0;
	is_rapid = false;

	can_rapid_shot = false;
}

void player_deinit()
{
	safe_delete(sprPlayer);
	for (int i = 0; i < 2; i++)
	{
		safe_delete(sprBullet[i]);
	}

	for (auto* bullet : player_bullets)
	{
		delete bullet;
	}
	player_bullets.clear();
}

//#define OLD_SHOT
#define NEW_SHOT

void shot_bullet()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	if (cursorPos.y < 380) cursorPos.y = 380;
	//float r = ToRadian(rand() % 360);
	//vec = {cosf(r), sinf(r)};
	VECTOR2 vec = cursorPos - player.pos;
	float length = vec2Length(vec);

	VECTOR2 a[9] = {
		{0, 0}, {100, 0}, {0, 50}, {-100, 0}, {0, -50},
		{70, 35},{70, -35},{-70, 35},{-70, -35},
	};

	for (int i = 0; i < bullet_lay_count; i++)
	{
#ifdef OLDSHOT
		{
			VECTOR2 vec = cursorPos - player.pos;

			auto* b = new Bullet();
			b->pos = player.pos;
			b->scale = { bullet_scale , bullet_scale };
			float f = (rand() / (float)RAND_MAX) * 2 - 1;//乱数設定
			float k = atan2f(vec.y, vec.x) + f * precision_rad;//拡散角度設定
			b->angle = k;
			b->angle += bullet_lay_gap * i - bullet_lay_gap * (bullet_lay_count - 1) / 2;//端になるほど斜めになる。
			vec = { cosf(b->angle), sinf(b->angle) };
			b->velocity = vec2Normalize(vec) * bullet_speed / system::FRAME_RATE;
			b->angle += ToRadian(180);
			b->pos += vec * shot_timer;
		}
#elif defined(NEW_SHOT)
		auto* b = new Bullet();
		b->pos = player.pos;
		b->scale = { bullet_scale , bullet_scale };
		b->end_pos = cursorPos + a[i];
		if (b->end_pos.y < 380)
		{
			b->end_pos.y = 380;
		}
		//b->end_pos.x = cursorPos.x;
		//b->end_pos.y = cursorPos.y + i * 40.0f - 40.0f * (bullet_lay_count - 1) / 2;
		float length3 = vec2Length(b->end_pos - b->pos);


		//float f = (rand() / (float)RAND_MAX) * 2 - 1;//乱数設定
		//float k = 0;// f* precision_rad;//拡散角度設定
		//k += bullet_lay_gap * i - bullet_lay_gap * (bullet_lay_count - 1) / 2;
		vec = b->end_pos - b->pos;
		b->angle = atan2f(vec.y, vec.x);
		b->velocity = vec2Normalize(vec) * bullet_speed / system::FRAME_RATE * length3 / length;

		b->spr = sprBullet[0];
		player_bullets.push_back(b);
		rapidShoted_bullet_count++;
#endif
	}
}
//#define SHOT_AIM_TARGET
//#define SHOT_AIM_AND_CLICK_TARGET

void player_shot_bullet()
{
#ifdef SHOT_AIM_TARGET
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	if (cursorPos.y < 380) cursorPos.y = 380;
	bool isShot = false;
	VECTOR2 shotPos;

	if (shot_timer > 1 / shot_per_second)
	{
		for (int i = 0; i < ENEMY_NUMBER; i++)
		{
			if (enemy[i].hp <= 0) continue;
			VECTOR2 k = enemy[i].pos - enemy[i].pivot;
			VECTOR2 lu, rd;
			lu.x = k.x * enemy[i].scale.x;
			lu.y = k.y * enemy[i].scale.y;
			rd.x = k.x + enemy[i].texSize.x * enemy[i].scale.x;
			rd.y = k.y + enemy[i].texSize.y * enemy[i].scale.y;
			if (Collision2D::circle_vs_rect(cursorPos, 20, lu, rd))
			{
				isShot = true;
				shotPos = enemy[i].pos;
			}
		}

		if (isShot)
		{
			shot_bullet();
			shot_timer = 0;
		}
	}
	shot_timer += 1 / system::FRAME_RATE;

#elif defined(SHOT_AIM_AND_CLICK_TARGET)

	if (TRG(0) & PAD_START)
	{
		if (shot_timer > 1 / shot_per_second)
		{
			shot_timer = 0;
			shot_bullet();
		}
		else if (cast_bullet == 0)
		{
			cast_bullet++;
		}
		rapid_timer = 0;

		shake(30, 45);
	}

	if (cast_bullet > 0 && shot_timer > 1 / shot_per_second)
	{
		cast_bullet--;
		shot_timer = 0;
		shot_bullet();
	}

	if ((STATE(0) & PAD_START) && can_rapid_shot)
	{
		if (is_rapid)
		{
			while (shot_timer > 1 / shot_per_second)
			{
				shot_timer -= 1 / shot_per_second;
				shot_bullet();
			}
		}
		else if (rapid_timer > rapid_coolDown)
		{
			is_rapid = true;
			shot_timer = 0;
		}
	}

	shot_timer += 1 / system::FRAME_RATE;
	rapid_timer += 1 / system::FRAME_RATE;

	if (TRG_RELEASE(0) & PAD_START)
	{
		rapidShoted_bullet_count = 0;
		is_rapid = false;
	}
#endif // SHOT_AIM_TARGET

}

void player_move()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	if (cursorPos.y < 380) cursorPos.y = 380;
	VECTOR2 vec = cursorPos - player.pos;
	cursor_pos = cursorPos;
	//player.pos.y += vec.y * 1000.0f / system::FRAME_RATE;
	float targetY = cursorPos.y - 300;
	player.pos.y += (targetY - player.pos.y) * 0.9f / system::FRAME_RATE;
	debug::setString("cursor : %f", cursorPos.y);
	player.pos.x = (SCREEN_H - player.pos.y) * 0.3584f / 0.9336f;
	player.scale.x = player.scale.y = 1.0f - ((SCREEN_H - player.pos.y) / SCREEN_H) * 0.2f;//遠くに行けば行くほど小さくなる。

	//player.angle = atan2f(vec.x, -vec.y);
}

void player_update()
{
	player_move();
	//player_shot_bullet();
	bullet_update();
	debug::setString("shot_per_second:%f", shot_per_second);
	debug::setString("bullet_count:%d", player_bullets.size());

	if (TRG(0) & PAD_UP)
	{
		can_rapid_shot = true;
	}

	//int enemy_count = 0;
	//for (int e = 0; e < ENEMY_NUMBER; e++)
	//{
	//	AttackCollision::player_vs_enemy(player, enemy[e]);
	//	for (auto*& bullet : player_bullets)
	//	{
	//		AttackCollision::enemy_vs_bullet(enemy[e], *bullet);
	//	}
	//	if (enemy[e].scale.x > 0.0001f)
	//	{
	//		enemy_count++;
	//	}
	//}
	//debug::setString("enemies : %d", enemy_count);

}

void player_render()
{
	primitive::line(player.pos, cursor_pos);

	sprite_render(
		sprPlayer,
		player.pos.x, player.pos.y,
		player.scale.x, player.scale.y,
		0, 0,
		player.texSize.x, player.texSize.y,
		player.pivot.x, player.pivot.y,
		player.angle
	);

	for (auto*& bullet : player_bullets)
	{
		sprite_render(
			bullet->spr,
			bullet->pos.x, bullet->pos.y,
			bullet->scale.x, bullet->scale.y,
			0, 0,
			bullet->texSize.x, bullet->texSize.y,
			32, 4,
			bullet->angle
		);
		primitive::circle(
			bullet->end_pos,
			20,
			{ 1, 0.3f },
			0,
			{ 0, 0, 1, 0.01f }
		);
	}
}

void set_canRapidShot(bool b)
{
	can_rapid_shot = b;
}

void set_shotPerSecond(float value)
{
	shot_per_second = value;
}

void delete_bullet()
{
	for (auto itr = player_bullets.begin(); itr != player_bullets.end();)
	{
		//画面外に出ていたら。
		if ((*itr)->pos.x + 32 < 0
			|| (*itr)->pos.x - 32 > SCREEN_W
			|| (*itr)->pos.y + 32 < 0
			|| (*itr)->pos.y - 32 > SCREEN_H)
		{
			delete* itr;
			itr = player_bullets.erase(itr);
		}
		else if ((*itr)->pos.x > (*itr)->end_pos.x)
		{
			delete* itr;
			itr = player_bullets.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void bullet_attack()
{
	for (auto* bullet : player_bullets)
	{
		for (int e = 0; e < ENEMY_NUMBER; e++)
		{
			if (bullet->end_pos.y - bullet->pos.y < enemy[e].texSize.y * enemy[e].scale.y * 0.5f)
			{//AttackCollision::enemy_vs_bullet(enemy[e][e], *bullet);

				VECTOR2 enemy_begin = enemy[e].pos - VECTOR2{ enemy[e].scale.x * enemy[e].pivot.x, enemy[e].scale.y * enemy[e].pivot.y };//enemy[e]の左上
				VECTOR2 enemy_size = { enemy[e].scale.x * enemy[e].texSize.x, enemy[e].scale.y * enemy[e].texSize.y };
				VECTOR2 bullet_center = bullet->pos;
				float bullet_radius = 32;
				if (Collision2D::circle_vs_rect(bullet_center, bullet_radius, enemy_begin, enemy_size) && enemy[e].hp > 0)
				{
					enemy[e].hp -= bullet->attack_damage;
					bullet->pos = { -500, -500 };//画面外に
				}
			}
		}
	}
}

void bullet_move()
{
	for (auto* bullet : player_bullets)
	{
		bullet->pos += bullet->velocity;
	}
}

void bullet_update()
{
	if (shot_timer > 3)
	{
		shot_timer = 0;
		auto* y1 = new Bullet_Y1();
		y1->pos = player.pos;
		y1->end_pos = { SCREEN_W, SCREEN_H };
		y1->velocity.x = 1;
		y1->spr = sprBullet[1];
		y1->texSize = { 64, 64 };
		player_bullets.push_back(y1);
	}
	shot_timer += 1 / system::FRAME_RATE;
	bullet_move();

	bullet_attack();

	delete_bullet();
}