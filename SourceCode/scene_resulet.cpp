//******************************************************************************
//
//
//      scene_resulet
//
//
//******************************************************************************

//----< インクルード >-----------------------------------------------------------
#include "all.h"

//------< 定数 >----------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
int resulet_state = 0;     // 状態
int resulet_timer = 0;     // タイマー
// 背景
OBJ2D resulet;
Sprite* sprresulet;

// ボタン
OBJ2D resulet_button[2];
Sprite* sprresulet_button[2];

//--------------------------------------
//  初期設定
//--------------------------------------
void resulet_init()
{
	resulet_state = 0;
	resulet_timer = 0;
}

//--------------------------------------
//  更新処理
//--------------------------------------
void resulet_update()
{
	switch (resulet_state)
	{
	case 0:
		//////// 初期設定 ////////
		sprresulet = sprite_load(L"Data/images/BackGround_resulet/ping");

		resulet_state++;
		/*fallthrough*/
	case 1:
		//////// パラメータの設定 ////////
		sprresulet = {};
		resulet.pos = { 0, 0 };
		resulet.scale = { 0, 0 };
		resulet.texSize = { 1920, 1080 };
		resulet.pivot = { 960, 540 };
		resulet.color = { 1, 1, 1, 1 };
		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);

		resulet_state++;
		/*fallthrough*/
	case 2:
		//////// 通常時 ////////
		if (TRG(0) & PAD_START)
		{
			nextScene = SCENE_TITLE;
			break;
		}


		break;
	}

	resulet_timer++;
}

//--------------------------------------
//  描画処理
//--------------------------------------
void resulet_render()
{
	

}

//--------------------------------------
//  終了処理
//--------------------------------------
void resulet_deinit()
{


}
