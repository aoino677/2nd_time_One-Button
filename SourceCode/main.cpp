//******************************************************************************
//
//
//      main
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include "all.h"

//------< namespace >-----------------------------------------------------------
using namespace GameLib;

//------< 変数 >----------------------------------------------------------------
//------------------------------------------------------------------------------
// TODO:06 現在のシーンを管理する変数を作成する
//------------------------------------------------------------------------------
int curScene = SCENE_NONE;
int nextScene = SCENE_TITLE;
int endScene = SCENE_RESULT;

//------------------------------------------------------------------------------
//  WinMain（エントリポイント）
//------------------------------------------------------------------------------
int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// 使用しない変数は記述しない
{
	// ゲームライブラリの初期設定
	GameLib::init(L"ゲームプログラミングⅠ", (int)SCREEN_W, (int)SCREEN_H, FALSE);

	// TODO_08_1 コメントアウトをする
	/*game_init();*/

	while (GameLib::gameLoop())
	{
		//------------------------------------------------------------------------------
		// TODO:08 シーン切り替え処理を作成する
		//------------------------------------------------------------------------------
		if (curScene != nextScene)
		{
			//現在のシーンの終了処理(xxxxx_deinit())を行う
			switch (curScene)
			{
			case SCENE_TITLE:
				title_deinit();
				break;

			case SCENE_GAME:
				game_deinit();
				break;

			case SCENE_RESULT:
				resulet_deinit();
				break;
			}

			//現在のシーンを変更
			curScene = nextScene;

			//現在のシーンの終了処理(xxxxx_init())を行う
			switch (curScene)
			{
			case SCENE_TITLE:
				title_init();
				break;
			case SCENE_GAME:
				game_init();
				break;

			case SCENE_RESULT:
				resulet_init();
				break;
			}
		}


		// 入力処理
		input::update();

		// 現在のシーンを更新・描画
		//------------------------------------------------------------------------------
		// TODO:09 curSceneの値に応じたシーンの更新・描画をする
		//------------------------------------------------------------------------------
		// TODO_09_1 コメントアウトをする
		/*game_update();
		game_render();*/

		
		switch (curScene)
		{
		case SCENE_TITLE:
				title_update();
				title_render();
				break;
		case SCENE_GAME:
				game_update();
				game_render();
				break;
		case SCENE_RESULT:
			resulet_update();
			resulet_render();
			break;
		}

		// デバッグ文字列を描画
		debug::display(1.0f, 1.0f, 1.0f, 1, 1);

		// バックバッファの内容を表示
		GameLib::present(1, 0);
	}
	//------------------------------------------------------------------------------
	// TODO:10 ゲームループ終了後にシーンの終了処理を呼ぶ
	//------------------------------------------------------------------------------
	// TODO_10_1 コメントアウトをする
	/*game_deinit();*/

	switch (curScene)
	{
	case SCENE_TITLE:
		title_deinit();
		break;

	case SCENE_GAME:
		game_deinit();
		break;

	case SCENE_RESULT:
		resulet_deinit();
		break;
	}

	// ゲームライブラリの終了処理
	GameLib::uninit();
}
