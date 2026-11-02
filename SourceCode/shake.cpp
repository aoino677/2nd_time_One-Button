#include "all.h"

extern int shakeTimer;
extern float shakePower;
extern float shakeX;
extern float shakeY;

void shake_update()
{
    if (shakeTimer > 0)
    {
        shakeTimer--;

        shakeX = (float)(rand() % ((int)shakePower * 2 + 1))
            -shakePower;

        shakeY = (float)(rand() % ((int)shakePower * 2 + 1))
            - shakePower;

        //è≠ÇµÇ∏Ç¬é„Ç≠Ç∑ÇÈ
        shakePower *= 0.9f;
    }
    else
    {
        shakeX = 0;
        shakeY = 0;
        shakePower = 0;
     }
}

//-------------------------------------------------------------------------------
void camera_shake(int time, float power)
{
    shakeTimer = time;
    shakePower = power;
}