#pragma once
#include "all.h"

void shake_player(int time, float power);
void shake_player_update();
void shake_background(int time, float power);
void shake_background_update();

extern int shakeTimer;
extern float shakePower;
extern float shakeX1;
extern float shakeY1;
extern float shakeX2;
extern float shakeY2;

