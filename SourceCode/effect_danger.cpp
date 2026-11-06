#include"effect_danger.h"

extern int player_hp;
float danger_clear = 0.0f;
float danger_timer = 0.0f;
float danger_wave = 0.0f;

void effect_danger_init()
{
    danger_clear = 0.0f;
    danger_timer = 0.0f;
    danger_wave = 0.0f;
}

void effect_danger_update()
{
    if (player_hp <= 3)
    {
        danger_timer += 0.05f;

        danger_wave = (sinf(danger_timer) + 1.0f) * 0.5f;
        danger_clear = 0.4f * danger_wave;
    }
    else
    {
        danger_clear = 0.0f;
        danger_timer = 0.0f;
    }
}

void effect_danger_render()
{
    if (player_hp <= 3)
    {
        primitive::rect(
            0.0f, 0.0f,
            1920.0f, 648.0f,
            0.0f, 0.0f,
            0.0f,
            1.0f, 0.0f, 0.0f, danger_clear
        );
    }
}