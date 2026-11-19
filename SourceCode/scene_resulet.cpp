#include "all.h"
#include "result_screen.h"

int resulet_state = 0;
int resulet_timer = 0;

void resulet_init()
{
    resulet_state = 0;
    resulet_timer = 0;
}

void resulet_update()
{
    switch (resulet_state)
    {
    case 0:
        resulet_state++;
        /*fallthrough*/
    case 1:
        GameLib::setBlendMode(Blender::BS_ALPHA);
        resulet_state++;
        /*fallthrough*/
    case 2:
        
        break;
    }

    resulet_timer++;
}

void resulet_render()
{
    GameLib::clear(1.0f, 0.2f, 0.2f);
    font::textOut(1, "PRESS SPACE KEY", 640, 300);
}

void resulet_deinit()
{
    
}