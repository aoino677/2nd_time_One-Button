#pragma once
#include "../GameLib/game_lib.h"
#include <vector>
#include "OBJ2D.h"

struct Bullet
{
	virtual ~Bullet() = default;
	GameLib::Sprite* spr;
	VECTOR2 pos;//現在位置
	VECTOR2 velocity;//速度
	VECTOR2 scale = { 1, 1 };//大きさ
	VECTOR2 texSize = { 64, 8 };//大きさ
	float angle = 0;//角度

	int attack_damage = 10;

	VECTOR2 end_pos;
	VECTOR2 accel_velocity;//加速度

 	virtual void update() = 0;
 	virtual void attack(OBJ2D[]) = 0;
	virtual void render();
	virtual bool is_dead();
};

struct Bullet_Y1 : Bullet
{
	VECTOR2 old_pos;
	int color_index;
	~Bullet_Y1()override = default;
	void update() override;
	void attack(OBJ2D[]) override;
};

struct Bullet_Shell : Bullet
{
	void update() override;
	void attack(OBJ2D[]) override;
};