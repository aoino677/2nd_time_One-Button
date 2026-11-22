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
#include "Background.h"
#include "cursor_main.h"
#include "shake.h"
#include "level_up.h"
#include "Hp.h"
#include "effect_danger.h"
#include "choice.h"
#include "Time_limit.h"
#include "power_efficacy.h"
#include "item.h"
#include "result_screen.h"
#include "effect_scene_change.h"
#include "effect_count_down.h"
#include "effect_game_start.h"


//------< 定数 >----------------------------------------------------------------
#define BGM_BATTLE 6
#define BGM_BATTLE_VOLUME (0.2f)
#define BGM_BOSS 7

//------< 変数 >----------------------------------------------------------------
int game_state = 0;     //状態
int game_timer = 0;     //タイマー
int time_limit = 0;       //制限時間

bool choice_pause = false;
bool pause = false;
//--------------------------------------
//  初期設定
//--------------------------------------
void game_init()
{
	game_state = 0;
	game_timer = 0;
	pause = false;
	choice_pause = false;
	player_init();
	enemy_init();
	pause_init();
	level_init();
	EXP_init();
	background_init();
	cursor_main_init();
	level_up_init();
	hp_init();
	effect_danger_init();
	choice_init();
	time_limit_init();
	item_init();
	efficacy_init();
	result_screen_init();
	countdown_init();
	effect_game_start_init();
	cursor_main_init();
	level_up_init();

	// ゲーム開始時はWindows標準カーソルを非表示
	while (ShowCursor(FALSE) >= 0)
	{
	}

	music::load(BGM_BATTLE, L"Data/Sounds/BGM/game_normal3.wav");
	music::load(BGM_BOSS, L"Data/Sounds/BGM/game_boss2.wav");

	music::play(BGM_BATTLE, true);
	music::setVolume(BGM_BATTLE, BGM_BATTLE_VOLUME);
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
		countdown_update();
		
		//リザルト表示中はゲーム停止
		if (result_active)
		{
			result_screen_update();
			cursor_main_update();
			break;
		}

		//////// 通常時 ////////
		pause_update();
		choice_update_pause();
		level_update();

		//========================================
		// ゲーム中
		//========================================
		
		
		if (pause == false && choice_pause == false)
		{
			if (countdown_finished())
			{
				time_limit_update();
				player_update();
				item_update();
				EXP_update();
				enemy_update();
				shake_update();
				effect_danger_update();
				cursor_main_update();
				effect_game_start_update();
				level_up_update();
			}
			
		}
			
		
		
		//仮リザルト
		if (TRG(0) & PAD_TRG1)
		{
			result_active = true;
			break;
		}

		//========================================
		// ポーズ中
		//========================================
		if (pause == true || choice_pause == true)
		{
			cursor_main_update();
			choice_update();
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

	effect_game_start_render();
	if (countdown_finished())
	{
		background_render();
		enemy_render();
		EXP_render();
		player_render();
		item_render();
		level_up_render();
		level_up_render();
		hp_render();
		effect_danger_render();
		time_limit_render();
		cursor_main_render();
		choice_render();
		pause_render(); // これ以上下に書かない
	}
	
    
	result_screen_render();
	if (pause == true || choice_pause == true)
	{
		cursor_main_render();
	}
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
	background_deinit();
	cursor_main_deinit();
	choice_deinit();
	item_deinit();
	result_screen_deinit();
	countdown_deinit();
	effect_game_start_deinit();
	cursor_main_deinit();
	level_up_deinit();
}