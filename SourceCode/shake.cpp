#include "shake.h"

int shake_timer1;
float Power1;
float shakeX1;
float shakeY1;
int shake_timer2;
float Power2;
float shakeX2;
float shakeY2;

void shake_player_update()
{
    if (shake_timer1 > 0)
    {
        shake_timer1--;

        shakeX1 = (float)(rand() % ((int)Power1 * 2 + 1))
            -Power1;

        shakeY1 = (float)(rand() % ((int)Power1 * 2 + 1))
            - Power1;

        //è≠ÇµÇ∏Ç¬é„Ç≠Ç∑ÇÈ
        Power1 *= 0.9f;
    }
    else
    {
        shakeX1 = 0;
        shakeY1 = 0;
        Power1 = 0;
     }
}

void shake_background_update()
{
    if (shake_timer2 > 0)
    {
        shake_timer2--;

        shakeX2 = (float)(rand() % ((int)Power2 * 2 + 1))
            - Power1;

        shakeY2 = (float)(rand() % ((int)Power2 * 2 + 1))
            - Power1;

        //è≠ÇµÇ∏Ç¬é„Ç≠Ç∑ÇÈ
        Power2 *= 0.9f;
    }
    else
    {
        shakeX2 = 0;
        shakeY2 = 0;
        Power2= 0;
    }
}

void shake_player(int time, float power)
{
    shake_timer1 = time;
    Power1 = power;
}

void shake_background(int time, float power)
{
    shake_timer2 = time;
    Power2 = power;
}