#pragma once
#include "all.h"

class Player
{
public:
    int hp;
    int attack;
    float posX = 960;
    float posY = 540;
};
  
void player_init();
void player_deinit();
void player_update();
void player_render();