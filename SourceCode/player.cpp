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
float shot_per_second = 5.0f;
float rapid_cool_down;

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
		shot_bullet();
	}
	if (STATE(0) & PAD_START)
	{
		if (shot_timer < 0)
		{
			if (count <= 0)
			{
				shot_timer = 0.5f;
			}
			shot_timer = 0.2f / (count + 1);
			if (shot_timer < 1 / shot_per_second) shot_timer = 1 / shot_per_second;
			shot_bullet();
		}
	}

	shot_timer -= 1 / 60.0f;

	if (TRG_RELEASE(0) & PAD_START)
	{
		 count = 0;
		 shot_timer = 0;
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