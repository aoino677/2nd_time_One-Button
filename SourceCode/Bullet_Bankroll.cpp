#include "Bullet_Bankroll.h"

Bullet_Bankroll::~Bullet_Bankroll()
{
}

void Bullet_Bankroll::update()
{
	angle += ToRadian(20) / system::FRAME_RATE;
}

void Bullet_Bankroll::attack(OBJ2D[])
{
}

void Bullet_Bankroll::render()
{
	sprite_render(
		spr,
		pos.x, pos.y,
		scale.x, scale.y,
		0, 0,
		texSize.x, texSize.y,
		pivot.x, pivot.y,
		angle
	);
}

bool Bullet_Bankroll::is_dead()
{
	return false;
}
