#include "all.h"

int shake_timer;
float Power;
float shakeX;
float shakeY;

void shake_update()
{
    if (shake_timer > 0)
    {
        shake_timer--;

        shakeX = (float)(rand() % ((int)Power * 2 + 1))
            -Power;

        shakeY = (float)(rand() % ((int)Power * 2 + 1))
            - Power;

        //è≠ÇµÇ∏Ç¬é„Ç≠Ç∑ÇÈ
        Power *= 0.9f;
    }
    else
    {
        shakeX = 0;
        shakeY = 0;
        Power = 0;
     }
}


void shake(int time, float power)
{
    shake_timer = time;
    Power = power;
}