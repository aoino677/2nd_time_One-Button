#pragma once

class Enemy
{
public:
    int hp;
    int attack;
    float posX = 360;
    float posY = 540;
};

void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();
