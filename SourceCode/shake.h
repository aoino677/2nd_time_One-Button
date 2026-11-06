#pragma once
#include "all.h"

void shake(int time, float power);
void shake_update();

extern int shakeTimer;
extern float shakePower;
extern float shakeX;
extern float shakeY;
