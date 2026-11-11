#pragma once
#include "../GameLib/game_lib.h"

struct Bullet
{
	GameLib::Sprite* spr;
	VECTOR2 pos;//åªç›à íu
	VECTOR2 end_pos;
	VECTOR2 velocity;//ë¨ìx
	VECTOR2 scale = { 1, 1 };//ëÂÇ´Ç≥
	VECTOR2 texSize = { 64, 8 };//ëÂÇ´Ç≥
	float angle = 0;//äpìx

	float attack_damage = 10;

 	virtual void render();
};

struct Bullet_Y1 : Bullet
{
	void render() override;
};