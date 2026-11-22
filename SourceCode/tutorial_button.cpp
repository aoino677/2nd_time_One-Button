#include "tutorial_button.h"
#include "OBJ2D.h"

int tutorial_number = 0;
Sprite** sprTutorial;

void tutorial_button_init()
{
	sprTutorial = new Sprite * [4];
	sprTutorial[0] = sprite_load(L"Data/Images/tutorial1.png");
	sprTutorial[1] = sprite_load(L"Data/Images/tutorial2.png");
	sprTutorial[2] = sprite_load(L"Data/Images/tutorial3.png");
	sprTutorial[3] = sprite_load(L"Data/Images/tutorial4.png");
	tutorial_number = 0;
}

void tutorial_button_deinit()
{
	safe_delete_array(sprTutorial);
}

void tutorial_button_update()
{

}

void tutorial_button_render()
{

}
