#include "all.h"
#include "player.h"

Sprite* sprPlayer;
Player player;
int player_state;
int player_timer;

void player_init()
{
	player_state = 0;
	player_timer = 0;
}

void player_deinit()
{
	safe_delete(sprPlayer);
}

void player_update()
{
	switch (player_state)
	{
	case 0:
		sprPlayer = sprite_load(L"Data/Images/player.png");
	
		break;
	//case 1:
		
	}
}

void player_render()
{
	sprite_render(
		sprPlayer,
		player.posX, player.posY
	);
}