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
#include "cursor_main.h"

//------< 定数 >----------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
Sprite* sprTitle;

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
	cursor_main_init();
	// ゲーム開始時はWindows標準カーソルを非表示
	while (ShowCursor(FALSE) >= 0)
	{
	}
}

//--------------------------------------
//  更新処理
//--------------------------------------
void title_update()
{
	switch (title_state)
	{
	case 0:
		sprTitle= sprite_load(L"Data/Images/title.png");
		title_state++;
		/*fallthrough*/
	case 1:
		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);
		title_state++;
		/*fallthrough*/
	case 2:
		title_button_update();
		cursor_main_update();
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

	sprite_render(
		sprTitle,
		0, 0
	);

	

	title_button_render();
	scene_change_render();
	cursor_main_render();
}

//--------------------------------------
//  終了処理
//--------------------------------------
void title_deinit()
{
	title_button_deinit();
	cursor_main_deinit();
}