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

	sprPlayer = sprite_load(L"Data/Images/player2.png");

	player.pos.x = SCREEN_W * 0.5f;
	player.pos.y = SCREEN_H * 0.5f;
}

void player_deinit()
{
	safe_delete(sprPlayer);
}

void player_update()
{
	//VECTOR2 cursorPos = { (float)getCursorPosX(), (float)getCursorPosY() };
	//VECTOR2 vec = cursorPos - player.pos;

}

void player_render()
{
	VECTOR2 cursorPos = { (float)getCursorPosX(), (float)getCursorPosY() };
	VECTOR2 vec = cursorPos - player.pos;

	primitive::line(player.pos, cursorPos);

	sprite_render(
		sprPlayer,
		player.pos.x, player.pos.y,
		1, 1,
		0, 0,
		128, 128,
		64, 64,
		atan2f(vec.x, -vec.y)
	);
}