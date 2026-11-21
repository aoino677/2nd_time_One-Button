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
#include "title_button.h"


//------< 定数 >----------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
int title_state = 0;     //状態
int title_timer = 0;     //タイマー

//--------------------------------------
//  初期設定
//--------------------------------------
void title_init()
{
	title_state = 0;
	title_timer = 0;
	title_button_init();
}

//--------------------------------------
//  更新処理
//--------------------------------------
void title_update()
{
	switch (title_state)
	{
	case 0:
		title_state++;
		/*fallthrough*/
	case 1:
		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);
		title_state++;
		/*fallthrough*/
	case 2:
		title_button_update();

	    break;
	}

	title_timer++;
}

//--------------------------------------
//  描画処理
//--------------------------------------
void title_render()
{
	// 画面をクリアする
	GameLib::clear(0.2f, 0.2f, 0.2f);

	primitive::rect(
		0, 0,
		1920, 1080,
		0, 0,
		0,
		1, 0, 0
	);

	font::textOut(1, "GAME", 640, 200);
	if ((title_timer / 20) % 2 == 0)
	{
		font::textOut(1, "PRESS SPACE KEY", 640, 300);
	}
	title_button_render();
}

//--------------------------------------
//  終了処理
//--------------------------------------
void title_deinit()
{
	title_button_deinit();
}