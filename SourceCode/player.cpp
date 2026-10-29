#include "all.h"
#include "player.h"
#include "Bullet.h"

#include "enemy.h"
#include "AttackCollision.h"
#include <vector>

Sprite* sprPlayer;
Sprite* sprBullet[3];
OBJ2D player;
std::vector<Bullet*> player_bullets;

int player_state;
int player_timer;

float shot_timer;			//弾発射の待機時間
float rapid_timer;			//完了したら連射に移行する
bool is_rapid;				//連射中か

bool can_rapid_shot;		//連射可能か
int rapidShoted_bullet_count;//連射した弾数
int cast_bullet;			//発射を待機している弾

float shot_per_second = 5.0f; //１秒間に発射する弾の数
float rapid_coolDown = 0.2f; //連射開始時間

float precision_rad = ToRadian(5); //狙った方向に飛ぶ精度

void player_init()
{
	player_state = 0;
	player_timer = 0;

	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet[0] = sprite_load(L"Data/Images/1yen.png");
	sprBullet[1] = sprite_load(L"Data/Images/10yen.png");
	sprBullet[2] = sprite_load(L"Data/Images/100yen.png");
	
	player.pos = PLAYER_POS;
	player.texSize = { 128, 128 };

	shot_timer = 0;
	rapidShoted_bullet_count = 0;
	is_rapid = false;

	can_rapid_shot = false;
}

void player_deinit()
{
	safe_delete(sprPlayer);
	for (int i = 0; i < 3; i++)
	{
		safe_delete(sprBullet[i]);
	}

	for (auto*& bullet : player_bullets)
	{
		delete bullet;
	}
	player_bullets.clear();
}

void delete_outWindow_bullet()
{
	for (auto itr =  player_bullets.begin(); itr != player_bullets.end();)
	{
		if ((*itr)->pos.x + 32 < 0
			|| (*itr)->pos.x - 32 > SCREEN_W
			|| (*itr)->pos.y + 32 < 0
			|| (*itr)->pos.y - 32 > SCREEN_H)
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

void shot_bullet()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	VECTOR2 vec = cursorPos - player.pos;
	//float r = ToRadian(rand() % 360);
	//vec = {cosf(r), sinf(r)};
	float f = (rand() / (float)RAND_MAX) * 2 - 1;
float k = atan2f(vec.y, vec.x) + f * precision_rad;
	vec = { cosf(k), sinf(k) };

	auto* b = new Bullet();
	b->pos = player.pos;
	b->velocity = vec2Normalize(vec) * 240 / 60.0f;
	b->angle = atan2f(vec.x, -vec.y);
	b->spr = sprBullet[(rand() % 3)];
//	b->angle = ToRadian(rand() % 360);
	player_bullets.push_back(b);
	rapidShoted_bullet_count++;
}

void player_shot_bullet()
{
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

	shot_timer += 1 / 60.0f;
	rapid_timer += 1 / 60.0f;

	if (TRG_RELEASE(0) & PAD_START)
	{
		rapidShoted_bullet_count = 0;
		is_rapid = false;
	}
}

void player_update()
{
	player_shot_bullet();
	for (auto*& bullet : player_bullets)
	{
		//bullet->velocity.y += 1.5f / 60.0f;
		bullet->pos += bullet->velocity;
	}
	delete_outWindow_bullet();
	debug::setString("shot_per_second:%f", shot_per_second);
	debug::setString("bullet_count:%d", player_bullets.size());

	if (TRG(0) & PAD_UP)
	{
		can_rapid_shot = true;
	}
	debug::setString("is rapid : %d", can_rapid_shot);

	int enemy_count = 0;
	for (int e = 0; e < ENEMY_NUMBER; e++)
	{
		AttackCollision::player_vs_enemy(player, enemy[e]);
		for (auto*& bullet : player_bullets)
		{
			AttackCollision::enemy_vs_bullet(enemy[e], *bullet);
		}
		if (enemy[e].scale.x > 0.0001f)
		{
			enemy_count++;
		}
	}
	debug::setString("enemies : %d", enemy_count);

}

void player_render()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	VECTOR2 vec = cursorPos - player.pos;

	primitive::line(player.pos, cursorPos);

	sprite_render(
		sprPlayer,
		player.pos.x, player.pos.y,
		1, 1,
		0, 0,
		player.texSize.x, player.texSize.y,
		player.texSize.x * 0.5f, player.texSize.y * 0.5f,
		atan2f(vec.x, -vec.y)
	);

	for (auto*& bullet : player_bullets)
	{
		sprite_render(
			bullet->spr,
			bullet->pos.x, bullet->pos.y,
			1, 1,
			0, 0,
			64, 64,
			32, 32,
			bullet->angle
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
