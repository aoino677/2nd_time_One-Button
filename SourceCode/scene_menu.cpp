//******************************************************************************
//
//
//      scene_menu
//
//
//******************************************************************************

//----< インクルード >-----------------------------------------------------------
#include "all.h"
#include "menu_button.h"
#include "effect_scene_change.h"
#include "effect_start_game.h"
#include "cursor.h"
//------< 定数 >----------------------------------------------------------------

//------< 変数 >----------------------------------------------------------------
int menu_state = 0;     //状態
int menu_timer = 0;     //タイマー
bool is_effect_cleaned_up = false;
bool is_wipe_out_cleaned_up = false;

Sprite* spr_menu_background;
float menu_background_posX;

//--------------------------------------
//  初期設定
//--------------------------------------
void menu_init()
{
	menu_state = 0;
	menu_timer = 0;
	is_effect_cleaned_up = false;
	is_wipe_out_cleaned_up = false;
	menu_background_posX = 0;

	menu_button_init();
	cursor_init();
	scene_wipe_init();

	spr_menu_background = sprite_load(L"Data/Images/result_back.png");
}

//--------------------------------------
//  更新処理
//--------------------------------------
void menu_update()
{
	switch (menu_state)
	{
	case 0:
		//////// 初期設定 ////////

		menu_state++;
		/*fallthrough*/
	case 1:
		//////// パラメータの設定 ////////

		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);

		menu_state++;
		/*fallthrough*/
	case 2:
		//////// 通常時 ////////

		scene_wipe_out_update();
		menu_button_update();
		cursor_update();

		if (is_scene_wipe_out_finished() && !is_wipe_out_cleaned_up)
		{
			//scene_wipe_deinit();
			is_wipe_out_cleaned_up = true;
		}
		menu_background_posX--;
		if (menu_background_posX < 0)menu_background_posX = SCREEN_W;
		break;
	}

	menu_timer++;
}

//--------------------------------------
//  描画処理
//--------------------------------------
void menu_render()
{
	// 画面をクリアする
	GameLib::clear(0.2f, 0.2f, 0.2f);

	//primitive::rect(
	//	0, 0,
	//	1920, 1080,
	//	0, 0,
	//	0,
	//	0, 1, 0
	//);

	sprite_render(
		spr_menu_background,
		menu_background_posX,0
		);

	sprite_render(
		spr_menu_background,
		menu_background_posX - SCREEN_W, 0
	);

	menu_button_render();

	if (!is_scene_change_finished())
	{
		scene_change_render();
	}
	else if (!is_effect_cleaned_up)
	{
		scene_change_deinit();
		is_effect_cleaned_up = true;
	}
	
	scene_wipe_out_render();

	cursor_render();
}

//--------------------------------------
//  終了処理
//--------------------------------------
void menu_deinit()
{
	menu_button_deinit();
	cursor_deinit();
	//scene_wipe_deinit();
}