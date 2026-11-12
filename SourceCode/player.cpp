#include "player.h"
#include "Bullet.h"
#include "enemy.h"
#include "AttackCollision.h"
#include "shake.h"
#include <vector>
#include "Collision2D.h"

float bullet_speed = BULLET_SPEED;//”­Ë‚·‚é’e‚Ì1•b“–‚½‚è‚Ì‘¬“x
float shot_per_second = 20.0f; //‚P•bŠÔ‚É”­Ë‚·‚é’e‚Ì”	
//float rapid_coolDown = RAPID_COOLDOWN; //˜AËŠJnŠÔ
float precision_rad = ToRadian(5); //‘_‚Á‚½•ûŒü‚É”ò‚Ô¸“x

float Y1_shot_timer = 0;

//float shot_timer;			//’e”­Ë‚Ì‘Ò‹@ŠÔ
//float rapid_timer;			//Š®—¹‚µ‚½‚ç˜AË‚ÉˆÚs‚·‚é
//bool is_rapid;				//˜AË’†‚©

//bool can_rapid_shot;		//˜AË‰Â”\‚©
//int rapidShoted_bullet_count;//˜AË‚µ‚½’e”
//int cast_bullet;			//”­Ë‚ğ‘Ò‹@‚µ‚Ä‚¢‚é’e

int bullet_lay_count = 9;
float bullet_lay_gap = ToRadian(7);//’e‚ÌŠÔŠu
#define BULLET_SCALE (0.5f)
float bullet_scale = BULLET_SCALE;//’e‚Ì‘å‚«‚³

float Y1_damage = Y1_DAMAGE;
float ingot_damage = INGOT_DAMAGE;

Sprite* sprPlayer;
Sprite* sprBullet[3];
OBJ2D player;
std::vector<Bullet*> player_bullets;

VECTOR2 cursor_pos{};

void player_init()
{
	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet[0] = sprite_load(L"Data/Images/bullet.png");
	sprBullet[1] = sprite_load(L"Data/Images/1yen.png");
	sprBullet[2] = sprite_load(L"Data/Images/500yen.png");

	player.pos = PLAYER_POS;
	player.texSize = PLAYER_TEX_SIZE;
	player.pivot = player.texSize / 2;
	//shot_timer = 0;
	//rapidShoted_bullet_count = 0;
	//is_rapid = false;
	//
	//can_rapid_shot = false;
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

#define OLD_SHOT
//#define NEW_SHOT

void shot_bullet()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	//if (cursorPos.y < 380) cursorPos.y = 380;
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
#ifdef OLD_SHOT
		{
			auto* b = new Bullet_Y1();
			b->spr = sprBullet[1];
			b->texSize = { 64, 64 };
			b->pos = player.pos;
			b->scale = { bullet_scale , bullet_scale };
			float f = (rand() / (float)RAND_MAX) * 2 - 1;//—”İ’è
			float k = atan2f(vec.y, vec.x) + f * precision_rad;//ŠgUŠp“xİ’è
			b->angle = k;
			b->angle += bullet_lay_gap * i - bullet_lay_gap * (bullet_lay_count - 1) / 2;//’[‚É‚È‚é‚Ù‚ÇÎ‚ß‚É‚È‚éB
			vec = { cosf(b->angle), sinf(b->angle) };
			b->velocity = vec2Normalize(vec) * bullet_speed / system::FRAME_RATE;
			b->angle += ToRadian(180);
			b->end_pos = { (float)SCREEN_W, (float)SCREEN_H };
			b->attack_damage = Y1_damage;
			//b->pos += vec * shot_timer;
			player_bullets.push_back(b);
		}
#elif defined(NEW_SHOT)
		auto* b = new Bullet_Y1();
		b->texSize = { 64, 64 };
		b->attack_damage = Y1_damage;
		b->pos = player.pos;
		b->pos.x += player.texSize.x - player.pivot.x;
		b->scale = { bullet_scale , bullet_scale };
		b->end_pos = cursorPos + a[i];
		//if (b->end_pos.y < 380)
		//{
		//	b->end_pos.y = 380;
		//}
		//b->end_pos.x = cursorPos.x;
		//b->end_pos.y = cursorPos.y + i * 40.0f - 40.0f * (bullet_lay_count - 1) / 2;
		float length3 = vec2Length(b->end_pos - b->pos);

		//float f = (rand() / static_cast<float>(RAND_MAX)) * 2 - 1;//—”İ’è
		//float k = f * precision_rad;//ŠgUŠp“xİ’è
		//b->end_pos = player.pos;
		//b->end_pos += { cosf(k) * length3, sinf(k) * length3 };
		vec = b->end_pos - b->pos;
		b->angle = atan2f(vec.y, vec.x);
		b->velocity = vec2Normalize(vec) * bullet_speed / system::FRAME_RATE * length3 / length;

		b->spr = sprBullet[1];
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

	if (TRG(0) & PAD_UP)
	{
		can_rapid_shot = true;
	}

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

	if (TRG(0) & PAD_UP)
	{
		can_rapid_shot = true;
	}
#endif // SHOT_AIM_TARGET

}

void player_move()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	//if (cursorPos.y < 380) cursorPos.y = 380;
	VECTOR2 vec = cursorPos - player.pos;
	cursor_pos = cursorPos;
	//player.pos.y += vec.y * 1000.0f / system::FRAME_RATE;
	float targetY = cursorPos.y; //- 300;
	player.pos.y += (targetY - player.pos.y) * PLAYER_EASING / system::FRAME_RATE;
	debug::setString("cursor : %f", cursorPos.y);
	player.pos.x = (SCREEN_H - player.pos.y) * 0.3584f / 0.9336f;
	player.scale.x = player.scale.y = 1.0f - ((SCREEN_H - player.pos.y) / SCREEN_H) * 0.2f;//‰“‚­‚És‚¯‚Îs‚­‚Ù‚Ç¬‚³‚­‚È‚éB

	//player.angle = atan2f(vec.x, -vec.y);
}

void player_update()
{
	player_move();
	//player_shot_bullet();
	bullet_update();
	debug::setString("shot_per_second:%f", shot_per_second);
	debug::setString("bullet_count:%d", player_bullets.size());



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
			bullet->texSize.x * 0.5f, bullet->texSize.y * 0.5f,
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

//void set_canRapidShot(bool b)
//{
//	//can_rapid_shot = b;
//}

void set_shotPerSecond(float value)
{
	shot_per_second = value;
}

void delete_bullet()
{
	for (auto itr = player_bullets.begin(); itr != player_bullets.end();)
	{
		//‰æ–ÊŠO‚Éo‚Ä‚¢‚½‚çB
		if ((*itr)->pos.x + 32 < 0
			|| (*itr)->pos.x - 32 > SCREEN_W
			|| (*itr)->pos.y + 32 < 0
			|| (*itr)->pos.y - 32 > SCREEN_H)
		{
			delete* itr;
			itr = player_bullets.erase(itr);
		}
		//else if ((*itr)->pos.x > (*itr)->end_pos.x)
		//{
		//	delete* itr;
		//	itr = player_bullets.erase(itr);
		//}
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
		bullet->attack(enemy);
	}
}

void bullet_move()
{
	for (auto* bullet : player_bullets)
	{
		bullet->update();
	}
}

void bullet_update()
{
	if (Y1_shot_timer > 3)
	{
		Y1_shot_timer = 0;
		auto* b_ingot = new Bullet_INGOT();
		b_ingot->pos = { static_cast<float>(getCursorPosX()),0 };
		b_ingot->end_pos = { SCREEN_W, SCREEN_H };
		b_ingot->spr = sprBullet[2];
		b_ingot->texSize = { 64, 64 };
		b_ingot->attack_damage = ingot_damage;
		b_ingot->accel_velocity.y = 3 / system::FRAME_RATE;
		player_bullets.push_back(b_ingot);
	}
	Y1_shot_timer += 1 / system::FRAME_RATE;
	bullet_move();

	bullet_attack();

	delete_bullet();
}