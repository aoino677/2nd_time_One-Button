#pragma once
#include"Bullet.h"

struct Bullet_Shell : Bullet
{
	Bullet_Shell() = default;
	~Bullet_Shell() = default;
	void update() override;
	void attack(OBJ2D[]) override;
};
