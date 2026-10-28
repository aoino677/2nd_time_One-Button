#pragma once
#include "all.h"

class Bullet
{
    VECTOR2 pos;
    VECTOR2 velocity;
};

class Player
{
public:
    int hp;
    int attack;
    VECTOR2 pos = { 960, 540 };
};

void player_init();
void player_deinit();
void player_update();
void player_render();