#include "Bullet.h"
using namespace GameLib;


void Bullet_Y1::render()
{
	sprite_render(spr, pos.x, pos.y, scale.x, scale.y);
}


void Bullet::render()
{
	sprite_render(spr, pos.x, pos.y, scale.x, scale.y);
}
