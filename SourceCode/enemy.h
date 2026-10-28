#pragma once

class Enemy
{
public:
    int hp;
    int attack;
    VECTOR2 pos{ 20,400 };

};

void enemy_init();
void enemy_deinit();
void enemy_update();
void enemy_render();
void enemy_walk();
