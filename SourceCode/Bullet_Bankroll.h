#pragma once
#include "Bullet.h"

struct Bullet_Bankroll : Bullet
{
	VECTOR2 pivot;
	struct after_image
	{
		VECTOR2 pos;
		VECTOR4 color;

	};
	~Bullet_Bankroll()override;
	void update() override;
	void attack(OBJ2D[]) override;
	void render() override;
	
	bool is_dead() override;

	std::vector<after_image> after_images;

private:
	std::vector<OBJ2D*> attacked_enemies;
};