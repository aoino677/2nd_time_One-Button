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
#include "cursor.h"
#include <cmath>

//------< 定数 >----------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
Sprite* sprTitle;
Sprite* sprTitle_logo;

int title_state = 0;     //状態
int title_timer = 0;     //タイマー

// ロゴの上下揺れ設定
float logo_base_x = 515.0f;
float logo_base_y = 270.0f;
float logo_float_amp = 10.0f;   // 上下に揺れる幅（px）
float logo_float_speed = 0.06f; // 揺れる速さ

//--------------------------------------
//  初期設定
//--------------------------------------
void title_init()
{
	title_state = 0;
	title_timer = 0;
	title_button_init();
	cursor_init();
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
		sprTitle = sprite_load(L"Data/Images/title.png");
		sprTitle_logo = sprite_load(L"Data/Images/title_logo.png");
		title_state++;
		/*fallthrough*/
	case 1:
		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);
		title_state++;
		/*fallthrough*/
	case 2:
		title_button_update();
		cursor_update();
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

	// ロゴの現在Y座標（上下にふわふわ揺らす）
	float logo_y = logo_base_y + sinf((float)title_timer * logo_float_speed) * logo_float_amp;

	sprite_render(
		sprTitle_logo,
		logo_base_x, logo_y,
		2.5, 2.5,
		0, 0,
		351, 162,
		175.5, 81,
		ToRadian(-7)
	);

	title_button_render();
	scene_change_render();
	cursor_render();
}

//--------------------------------------
//  終了処理
//--------------------------------------
void title_deinit()
{
	safe_delete(sprTitle);
	safe_delete(sprTitle_logo);
	title_button_deinit();
	cursor_deinit();
}