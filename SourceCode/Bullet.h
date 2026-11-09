#pragma once
#include "../GameLib/game_lib.h"

struct Bullet
{
	GameLib::Sprite* spr = nullptr;
	VECTOR2 pos;//åªç›à íu
	VECTOR2 end_pos;
	VECTOR2 velocity;//ë¨ìx
	VECTOR2 scale = { 1, 1 };//ëÂÇ´Ç≥
	float angle = 0;//äpìx
};