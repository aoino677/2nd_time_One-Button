//******************************************************************************
//
//
//      scene_title
//
//
//******************************************************************************

//----< インクルード >-----------------------------------------------------------
#include "all.h"
#include "effect_scene_change.h"

//------< 定数 >----------------------------------------------------------------

//------< 変数 >----------------------------------------------------------------
int menu_state = 0;     // 状態
int menu_timer = 0;     // タイマー
bool is_effect_cleaned_up = false;

//--------------------------------------
//  初期設定
//--------------------------------------
void menu_init()
{
	menu_state = 0;
	menu_timer = 0;
	is_effect_cleaned_up = false;
}

//--------------------------------------
//  更新処理
//--------------------------------------
void menu_update()
{
	if (!is_scene_change_finished())
	{
		scene_change_update();
		return; //エフェクトが終わるまではメニューの操作を受け付けない
	}
	
	
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
		if (TRG(0) & PAD_START)
		{
			nextScene = SCENE_GAME;
			break;
		}




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

	primitive::rect(
		0, 0,
		1920, 1080,
		0, 0,
		0, 
		0, 1, 0
	);

	font::textOut(1, "MENU", 640, 200);
	
	if (!is_scene_change_finished())
	{
		scene_change_render();
	}
	else if (!is_effect_cleaned_up)
	{
		scene_change_deinit();
		is_effect_cleaned_up = true;
	}
}

//--------------------------------------
//  終了処理
//--------------------------------------
void menu_deinit()
{


}