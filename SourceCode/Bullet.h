#pragma once
#include "../GameLib/game_lib.h"

struct Bullet
{
	GameLib::Sprite* spr;
	VECTOR2 pos;//åªç›à íu
	VECTOR2 velocity;//ë¨ìx
	float angle;//äpìx
};