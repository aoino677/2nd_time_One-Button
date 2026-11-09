#include "power_efficacy.h"
#include "player.h"
#include "Hp.h"
#include "Bullet.h"
#include "cursor_main.h"
#include <ctime>
#include <stdlib.h>

// 番号でポスが変わる
void Choice_num(int choice_num)
{
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		switch (choice_num)
		{
		case 0:
			power_efficacy[i].pos = { 450, 500 };
			break;
		case 1:
			power_efficacy[i].pos = { 950, 500 };
			break;
		case 2:
			power_efficacy[i].pos = { 1450, 500 };
			break;
		}
	}
}

// プレイヤーのHPが上がる
void HP(int choice_num)
{
	srand((unsigned int)time(NULL));

	player_hp += rand() % HP_MAX + 1;
	Choice_num(choice_num);
}
// 球の発射スピード
void Bullet_speed(int choice_num)
{
	bullet_speed += rand() % 50 + BULLET_SPEED;
	Choice_num(choice_num);
}
// 球の発射個数
void Bullet_quantity(int choice_num)
{
	bullet_lay_count++;
	Choice_num(choice_num);
}
// 球の大きさ
void Bullet_scale(int choice_num)
{
	bullet_scale += 0.01f;
	Choice_num(choice_num);
}
// 弾の狙った方向に飛ぶ精度
void Bullet_rad(int choice_num)
{
	precision_rad -= ToRadian(0.5f);
	Choice_num(choice_num);
}
// 弾の間隔
void Bullet_diffusion(int choice_num)
{
	bullet_lay_gap += ToRadian(3);
	Choice_num(choice_num);
}

// はずれ
void loser(int choice_num)
{
	player_hp += 0;
	bullet_speed += 0;
	bullet_lay_count += 0;
	precision_rad += 0;
	bullet_lay_gap += 0;
	Choice_num(choice_num);
}