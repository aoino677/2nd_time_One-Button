#include "all.h"
#include "player.h"
#include <vector>

Sprite* sprPlayer;
Sprite* sprBullet;
OBJ2D player;
std::vector<Bullet*> player_bullets;

int player_state;
int player_timer;

float shot_timer;			//’e”­ŽË‚Ì‘Ò‹@ŽžŠÔ
float rapid_timer;			//Š®—¹‚µ‚½‚ç˜AŽË‚ÉˆÚs‚·‚é
bool is_rapid;				//˜AŽË’†‚©

bool can_rapid_shot;		//˜AŽË‰Â”\‚©
int rapidShoted_bullet_count;//˜AŽË‚µ‚½’e”
int cast_bullet;			//”­ŽË‚ð‘Ò‹@‚µ‚Ä‚¢‚é’e

float shot_per_second = 40.0f; //‚P•bŠÔ‚É”­ŽË‚·‚é’e‚Ì”
float rapid_coolDown = 0.2f; //˜AŽËŠJŽnŽžŠÔ

void player_init()
{
	player_state = 0;
	player_timer = 0;

	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet = sprite_load(L"Data/Images/10yen.png");

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
	safe_delete(sprBullet);

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

	auto* b = new Bullet();
	b->pos = player.pos;
	b->velocity = vec2Normalize(vec) * 240 / 60.0f;
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
		//bullet->velocity.y += 1 / 60.0f;
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
			sprBullet,
			bullet->pos.x, bullet->pos.y,
			1, 1,
			0, 0,
			64, 64,
			32, 32
		);
	}
}

void set_canRapidShot(bool b)
{
	can_rapid_shot = b;
}

void set_shotPerSecond(int value)
{
	shot_per_second = value;
}
