#include "all.h"
#include "result_screen.h"

int farame_timer;
int second_timer;

void time_limit_init()
{
    farame_timer = 0;
    second_timer = 180;
}

void time_limit_update()
{
    farame_timer++;

    if (farame_timer >= 60)
    {
        second_timer--;
        farame_timer = 0;
    }

    if (second_timer <= 0)
    {
        second_timer = 0;
        result_active = true;
    }
}

void time_limit_render()
{
    float bar_x = 495.0f;
    float bar_y = 100.0f;
    float bar_width = 750.0f;
    float bar_height = 30.0f;

    float rate = (float)second_timer / 180.0f;

    // ”wŒi
    primitive::rect(
        bar_x, bar_y,
        bar_width, bar_height,
        0.0f, 0.0f, 0.0f,
        0.15f, 0.15f, 0.15f, 1.0f,
        false
    );

    // Žc‚èŽžŠÔ
    primitive::rect(
        bar_x, bar_y,
        bar_width * rate, bar_height,
        0.0f, 0.0f, 0.0f,
        0.2f, 0.9f, 0.3f, 1.0f,
        false
    );
}