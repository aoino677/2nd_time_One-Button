#include "all.h"
#include "result_button.h"

int resulet_state = 0;
int resulet_timer = 0;
Sprite* sprresult;
Sprite* sprResult_logo;

void resulet_init()
{
    resulet_state = 0;
    resulet_timer = 0;
    result_button_init();
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
        sprresult = sprite_load(L"Data/Images/result_screen.png");
        sprResult_logo = sprite_load(L"Data/Images/result.png");
        
        resulet_state++;
        /*fallthrough*/
    case 2:
        //result_button_update();
        if (TRG(0) & PAD_TRG1)
        {
            nextScene = SCENE_TITLE;
            break;
        }
        break;
    }

    resulet_timer++;
}

void resulet_render()
{
    GameLib::clear(1.0f, 0.2f, 0.2f);
    font::textOut(1, "PRESS SPACE KEY", 640, 300);
    
    sprite_render(
        sprresult,
        0, 0
    );

    sprite_render(
        sprResult_logo,
        230, 0
    );

    result_button_render();
}

void resulet_deinit()
{
    safe_delete(sprresult);
    result_button_deinit();
}