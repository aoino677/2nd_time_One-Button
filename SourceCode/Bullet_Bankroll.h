#pragma once
#include "Bullet.h"

struct Bullet_Bankroll : Bullet
{
	VECTOR2 pivot;
	struct after_image
	{
		VECTOR2 pos;
		VECTOR2 pivot;
		float angle;
		VECTOR4 color;
	};
	Bullet_Bankroll() = default;
	~Bullet_Bankroll()override;
	void update() override;
	void attack(OBJ2D[]) override;
	void render() override;
	
	bool is_dead() override;

	std::vector<after_image> after_images;

	float roll_speed;
	float roll_max;

	bool is_hit_least_once = false;//1‰ñ‚«‚è‚Ì”»’è

private:
	std::vector<OBJ2D*> attacked_enemies;
};