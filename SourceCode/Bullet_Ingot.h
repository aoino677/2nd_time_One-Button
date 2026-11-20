#pragma once
#include "Bullet.h"

struct Bullet_Ingot : Bullet
{
	enum Type
	{
		Ingot, Issen
	};
	Bullet_Ingot(Type type);
	struct after_image
	{
		VECTOR2 pos;
		VECTOR4 color;
	};
	~Bullet_Ingot()override;
	void update() override;
	void attack(OBJ2D[]) override;
	void render() override;

	bool is_dead() override;

	std::vector< after_image> after_images;

#ifdef CAN_RAPIDSHOT_BULLET_INGOT
	struct Ingot
	{
		VECTOR2 pos;
		VECTOR2 velocity;
	};

	float timer;

private:
	std::vector<Ingot*> ingots;

#endif
private:
	std::vector<OBJ2D*> attacked_enemies;
};