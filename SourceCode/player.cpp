#include "player.h"
#include "Bullet.h"
#include "enemy.h"
#include "AttackCollision.h"
#include "shake.h"
#include <vector>
#include "Collision2D.h"
#include <random>

#define IMAGE_COLOR_COUNT 5

float bullet_speed = BULLET_SPEED;//î≠éÀÇ∑ÇÈíeÇÃ1ïbìñÇΩÇËÇÃë¨ìx
float shot_per_second = 20.0f; //ÇPïbä‘Ç…î≠éÀÇ∑ÇÈíeÇÃêî	
//float rapid_coolDown = RAPID_COOLDOWN; //òAéÀäJénéûä‘
float precision_rad = ToRadian(5); //ë_Ç¡ÇΩï˚å¸Ç…îÚÇ‘ê∏ìx

float Y1_shot_timer = 0;

//float shot_timer;			//íeî≠éÀÇÃë“ã@éûä‘
//float rapid_timer;			//äÆóπÇµÇΩÇÁòAéÀÇ…à⁄çsÇ∑ÇÈ
//bool is_rapid;				//òAéÀíÜÇ©

//bool can_rapid_shot;		//òAéÀâ¬î\Ç©
//int rapidShoted_bullet_count;//òAéÀÇµÇΩíeêî
//int cast_bullet;			//î≠éÀÇë“ã@ÇµÇƒÇ¢ÇÈíe

int bullet_lay_count = 1;
float bullet_lay_gap = ToRadian(7);//íeÇÃä‘äu
#define BULLET_SCALE (1.0f)
float bullet_scale = BULLET_SCALE;//íeÇÃëÂÇ´Ç≥

float Y1_damage = Y1_DAMAGE;
float ingot_damage = INGOT_DAMAGE;

Sprite* sprPlayer;
Sprite* sprBullet[3];
OBJ2D player;
std::vector<Bullet*> player_bullets;

static const std::pair<VECTOR3, VECTOR3> afterimagecolors[IMAGE_COLOR_COUNT]
{
	{{1.0f, 0.5f, 0.75f},{0.2f, 0, 1}} ,		// Pink to Blue
	{{1.0f, 0.75f, 0.0f},{1.0f, 0.1f, 0.0f}} ,	// Yellow to Red
	{{0.2f, 0.8f, 0.0f},{1.0f, 1.0f, 1.0f}} ,	// Green to White
	{{0.2f, 0.7f, 1.0f},{1.0f, 0.9f, 0.75f}} ,	// Blue to Cream Yellow
	{{0.8f, 0.8f, 0.8f},{0.7f, 0.5f, 1.0f}} ,	// Gray to Purple
};

#define Y1_AFTERIMAGE_ALIVETIME (0.5f)

std::vector<Bullet_Y1_AfterImage*> y1_after_images;

VECTOR2 cursor_pos{};

void set_vector_Bullet_Y1_AfterImage(Bullet_Y1_AfterImage* image)
{
	y1_after_images.push_back(image);
}

void player_init()
{
	sprPlayer = sprite_load(L"Data/Images/player.png");
	sprBullet[0] = sprite_load(L"Data/Images/bullet.png");
	sprBullet[1] = sprite_load(L"Data/Images/bullet.png");
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
	for (int i = 0; i < 3; i++)
	{
		safe_delete(sprBullet[i]);
	}

	for (auto* bullet : player_bullets)
	{
		delete bullet;
	}

	for (auto* image : y1_after_images)
	{
		delete image;
	}
	player_bullets.clear();
}

#define OLD_SHOT
//#define NEW_SHOT

void shot_bullet()
{
	VECTOR2 cursorPos = { static_cast<float>(getCursorPosX()), static_cast<float>(getCursorPosY()) };
	//if (cursorPos.y < 380) cursorPos.y = 380;
#ifdef NEW_SHOT
	//float r = ToRadian(rand() % 360);
	//vec = {cosf(r), sinf(r)};

	//VECTOR2 vec = cursorPos - player.pos;
	//float length = vec2Length(vec);
#endif

	VECTOR2 a[9] = {
		{0, 0}, {100, 0}, {0, 50}, {-100, 0}, {0, -50},
		{70, 35},{70, -35},{-70, 35},{-70, -35},
	};
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> distribution(-precision_rad, precision_rad);

	for (int i = 0; i < bullet_lay_count; i++)
	{
#ifdef OLD_SHOT
		{
			VECTOR2 vec = cursorPos - player.pos;
			auto* b = new Bullet_Y1();
			b->spr = sprBullet[1];
			b->texSize = { 64, 8 };
			b->pos = player.pos;
			b->scale = { bullet_scale , bullet_scale };
			//float f = (rand() / (float)RAND_MAX) * 2 - 1;//óêêîê›íË
			//float k = atan2f(vec.y, vec.x) +f * precision_rad;//ägéUäpìxê›íË
			//b->angle = k;
			b->angle = distribution(mt);
			b->angle += atan2f(vec.y, vec.x);
			b->angle += bullet_lay_gap * i - bullet_lay_gap * (bullet_lay_count - 1) / 2.0f;//í[Ç…Ç»ÇÈÇŸÇ«éŒÇﬂÇ…Ç»ÇÈÅB
			vec = { cosf(b->angle), sinf(b->angle) };
			b->velocity = vec2Normalize(vec) * bullet_speed / system::FRAME_RATE;
			b->angle += ToRadian(180);
			b->end_pos = { (float)SCREEN_W, (float)SCREEN_H };
			b->attack_damage = Y1_damage;
			b->color_index = rand() % IMAGE_COLOR_COUNT;
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

		//float f = (rand() / static_cast<float>(RAND_MAX)) * 2 - 1;//óêêîê›íË
		//float k = f * precision_rad;//ägéUäpìxê›íË
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

void shot_bullet_Ingot()
{
	auto* b_ingot = new Bullet_INGOT();
	b_ingot->pos = { static_cast<float>(getCursorPosX()),0 };
	b_ingot->end_pos = { SCREEN_W, SCREEN_H };
	b_ingot->spr = sprBullet[2];
	b_ingot->texSize = { 64, 64 };
	b_ingot->attack_damage = ingot_damage;
	//TODO:å„Ç≈è¡Ç∑Ç©Ç‡
	b_ingot->attack_damage *= Y1_damage / Y1_DAMAGE;
	b_ingot->accel_velocity.y = 3 / system::FRAME_RATE;
	player_bullets.push_back(b_ingot);
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
	player.scale.x = player.scale.y = 1.0f - ((SCREEN_H - player.pos.y) / SCREEN_H) * 0.2f;//âìÇ≠Ç…çsÇØÇŒçsÇ≠ÇŸÇ«è¨Ç≥Ç≠Ç»ÇÈÅB

	//player.angle = atan2f(vec.x, -vec.y);
}

void player_update()
{
	player_move();
	//player_shot_bullet();
	bullet_update();

#ifdef _DEBUG
	debug::setString("[player debug]------------------------");
	debug::setString("shot_per_second:%f", shot_per_second);
	debug::setString("bullet_speed:%f", bullet_speed);
	debug::setString("bullet_attack:%f", Y1_damage);
	debug::setString("bullet_scale:%f", bullet_scale);
	debug::setString("bullet_lay_count:%d", bullet_lay_count);
	debug::setString("bullet_lay_gap:%f", ToDegree(bullet_lay_gap));
	debug::setString("bullet_precision:%f", ToDegree(precision_rad));
	debug::setString("");
	debug::setString("bullet_count:%d", player_bullets.size());
	debug::setString("after_images:%d", y1_after_images.size());
	debug::setString("--------------------------------------");
#endif // DEBUG

	if (TRG(0) & PAD_LEFT)
	{
		bullet_lay_count++;
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

	for (auto* image : y1_after_images)
	{
		float t = (Y1_AFTERIMAGE_ALIVETIME - image->timer) / Y1_AFTERIMAGE_ALIVETIME;// îÕàÕÅFÇOÅ`ÇP ÇPÇ»ÇÁï\é¶Ç≥ÇÍÇÈÅB
		float k = t * 2; if (k > 1) { k = 1; }//ï‚ê≥
		VECTOR3 color = afterimagecolors[image->color_index].first * k + afterimagecolors[image->color_index].second * (1 - k);

		sprite_render(
			sprBullet[0],
			image->pos.x, image->pos.y,
			1, 1,
			0, 0,
			64, 8,
			64 * 0.5f, 8 * 0.5f,
			image->angle,
			color.x, color.y, color.z, t
		);
	}

	for (auto* bullet : player_bullets)
	{
		bullet->render();
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
		//âÊñ äOÇ…èoÇƒÇ¢ÇΩÇÁÅB
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

	for (auto itr = y1_after_images.begin(); itr != y1_after_images.end();)
	{
		(*itr)->timer += 1 / system::FRAME_RATE;
		if ((*itr)->timer > Y1_AFTERIMAGE_ALIVETIME)
		{
			delete *itr;
			itr = y1_after_images.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void bullet_update()
{
#ifdef _DEBUG
	if (TRG(0) & PAD_LEFT)
	{
		shot_bullet_Ingot();
	}
#endif
	bullet_move();

	bullet_attack();

	delete_bullet();
}

Bullet_Y1_AfterImage::Bullet_Y1_AfterImage(const VECTOR2& pos, const float angle, const int c) : pos(pos), angle(angle), color_index(c) {}
