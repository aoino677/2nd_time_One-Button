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

	sprPlayer = sprite_load(L"Data/Images/player.png");
}

void player_deinit()
{
	safe_delete(sprPlayer);
}

void player_update()
{
	VECTOR2 cursorPos = { (float)getCursorPosX(), (float)getCursorPosY() };
	cursorPos;
}

void player_render()
{
	sprite_render(
		sprPlayer,
		player.posX, player.posY
	);
}