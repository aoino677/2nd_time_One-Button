#pragma once
#include "../GameLib/game_lib.h"
#include <vector>
#include "OBJ2D.h"

struct Bullet
{
	virtual ~Bullet() = default;
	GameLib::Sprite* spr;
	VECTOR2 pos;//åªç›à íu
	VECTOR2 end_pos;
	VECTOR2 velocity;//ë¨ìx
	VECTOR2 scale = { 1, 1 };//ëÂÇ´Ç≥
	VECTOR2 texSize = { 64, 8 };//ëÂÇ´Ç≥
	float angle = 0;//äpìx

	float attack_damage = 10;

 	virtual void update() = 0;
 	virtual void attack(OBJ2D&) = 0;
};

struct Bullet_Y1 : Bullet
{
	~Bullet_Y1()override = default;
	void update() override;
	void attack(OBJ2D&) override;
};

struct Bullet_INGOT : Bullet
{
	~Bullet_INGOT()override = default;
	void update() override;
	void attack(OBJ2D&) override;

private:
	std::vector<OBJ2D*> attacked_enemies;
};