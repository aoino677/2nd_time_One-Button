#include "all.h"
#include "player.h"
#include <vector>

Sprite* sprPlayer;
Sprite* sprBullet;
Player player;
std::vector<Bullet*> player_bullets;

int player_state;
int player_timer;

float shot_timer;
float rapid_timer;
int count;
bool is_rapid;
int cast_bullet;

float shot_per_second = 4.0f; //‚P•bŠÔ‚É”­ŽË‚·‚é’e‚Ì”
float rapid_coolDown = 0.75f; //˜AŽËŠJŽnŽžŠÔ

void player_init()
{
	player_state = 0;
	player_timer = 0;

	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet = sprite_load(L"Data/Images/bullet.png");

	player.pos.x = SCREEN_W * 0.5f;
	player.pos.y = SCREEN_H * 0.5f;

	shot_timer = 0;
	count = 0;
	is_rapid = false;
}

void player_deinit()
{
	safe_delete(sprPlayer);
	safe_delete(sprBullet);

	for (auto*& bullet : player_bullets)
	{
		delete bullet;
	}
	player_bullets.clear();
}

void shot_bullet()
{
	VECTOR2 cursorPos = { (float)getCursorPosX(), (float)getCursorPosY() };
	VECTOR2 vec = cursorPos - player.pos;
	auto* b = new Bullet();
	b->pos = player.pos;
	b->velocity = vec2Normalize(vec) * 200 / 60.0f;
	player_bullets.push_back(b);
	count++;
}

void player_update()
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
	
	if (cast_bullet && shot_timer > 1 / shot_per_second)
	{
		cast_bullet--;
		shot_timer = 0;
		shot_bullet();
	}

	if (STATE(0) & PAD_START)
	{
		if (is_rapid)
		{
			if (shot_timer > 1 / shot_per_second)
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
	debug::setString("%f", shot_timer);
	debug::setString("%d", cast_bullet);

	if (TRG_RELEASE(0) & PAD_START)
	{
		 count = 0;
		 is_rapid = false;
	}

	for (auto*& bullet : player_bullets)
	{
		bullet->pos += bullet->velocity;
	}

}

void player_render()
{
	VECTOR2 cursorPos = { (float)getCursorPosX(), (float)getCursorPosY() };
	VECTOR2 vec = cursorPos - player.pos;

	primitive::line(player.pos, cursorPos);

	sprite_render(
		sprPlayer,
		player.pos.x, player.pos.y,
		1, 1,
		0, 0,
		128, 128,
		64, 64,
		atan2f(vec.x, -vec.y)
	);

	for (auto*& bullet : player_bullets)
	{
		sprite_render(
			sprBullet,
			bullet->pos.x, bullet->pos.y,
			1, 1,
			0, 0,
			64, 64,
			32, 32
		);
	}
}