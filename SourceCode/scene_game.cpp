//******************************************************************************
//
//
//      scene_game
//
//
//******************************************************************************

//----< インクルード >-----------------------------------------------------------
#include "all.h"
#include "player.h"
#include "enemy.h"
#include "Pause.h"
#include "Level.h"
#include "EXP.h"

//------< 定数 >----------------------------------------------------------------


//------< 変数 >----------------------------------------------------------------
int game_state = 0;     // 状態
int game_timer = 0;     // タイマー
int time_limit = 0;       // 制限時間

bool pause = false;
//--------------------------------------
//  初期設定
//--------------------------------------
void game_init()
{
	game_state = 0;
	game_timer = 0;
	time_limit = 600000;
	pause = false;
	player_init();
	enemy_init();
	pause_init();
	level_init();
	EXP_init();
}

//--------------------------------------
//  更新処理
//--------------------------------------
void game_update()
{
	switch (game_state)
	{
	case 0:
		//////// 初期設定 ////////

		game_state++;
		/*fallthrough*/
	case 1:
		//////// パラメータの設定 ////////

		// ブレンドモードの設定
		GameLib::setBlendMode(Blender::BS_ALPHA);

		game_state++;
		/*fallthrough*/
	case 2:
		//////// 通常時 ////////
		pause_update();
		
		//========================================
	   // ゲーム中
	   //========================================

		if (pause == false)
		{
			time_limit--;
			player_update();
			EXP_update();
			enemy_update();
			level_update();
		}
		if (TRG(0) & PAD_TRG1)
		{
			nextScene = SCENE_RESULT;
			break;
		}
		else if (time_limit == 0)
		{
			time_limit = 0;
			nextScene = SCENE_RESULT;
			break;
		}
	//========================================
	 // ポーズ中
	 //========================================
		if (pause == true)
		{

		}

		break;
	}

	game_timer++;
}

//--------------------------------------
//  描画処理
//--------------------------------------
void game_render()
{
	// 画面をクリアする
	GameLib::clear(0.2f, 0.0f, 0.75f);
	debug::setString("title_state:%d", time_limit);

	EXP_render();
	player_render();
	enemy_render();
	pause_render();
	font::textOut(2, "GAME", 100, 200, 2, 2);

}

//--------------------------------------
//  終了処理
//--------------------------------------
void game_deinit()
{
	player_deinit();
	enemy_deinit();
	pause_deinit();
	EXP_deinit();
}
