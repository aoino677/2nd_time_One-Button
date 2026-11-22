#include "result_score.h"
#include "choice.h"
#include "enemy.h"
#include "Hp.h"

#define SE_DRUMROLL     26
#define SE_SCORE_FINISH 27

Sprite* sprNumber = nullptr;

Sprite* sprRankS = nullptr;
Sprite* sprRankA = nullptr;
Sprite* sprRankB = nullptr;
Sprite* sprRankC = nullptr;
Sprite* sprRankD = nullptr;
Sprite* sprRankE = nullptr;

Sprite* sprResultText1 = nullptr; //倒した敵
Sprite* sprResultText2 = nullptr; //タワーHP
Sprite* sprResultText3 = nullptr; //レベル

int result_score_state = 0;

//最終目標値
int score = 0;       //総合スコア
int score_kill = 0;  //キル数
int score_level = 0; //レベル
int score_hp = 0;    //タワーHP（最終残量）

//タワーHPのカウントダウン開始値
int start_hp = 100;

//カウントアップ・カウントダウン中の現在表示値
int disp_score = 0;
int disp_kill = 0;
int disp_level = 0;
int disp_hp = 0;

//各項目のカウント経過フレーム数
int count_frame_score = 0;
int count_frame_kill = 0;
int count_frame_level = 0;
int count_frame_hp = 0;

//終わるまでの目標フレーム数
const int COUNT_DURATION = 120;

//0:キル数, 1:タワーHP, 2:レベル, 3:総合スコア, 4:全完了
int active_item = 0;

int score_W = 64;
int score_H = 64;

// 総合スコア用SE管理フラグ
bool drumroll_loaded = false;
bool drumroll_playing = false;
bool score_finish_se_played = false;
bool total_score_calculated = false;

// 総合スコアの計算
int calc_total_score()
{
	int hp_bonus = 0;
	

	int total = score_kill + score_hp / max_hp * 100 + score_level;
	if (total < 0)
	{
		total = 0;
	}
	return total;
}

// ドラムロール停止＆完了SE再生の共通処理
void finish_score_drumroll()
{
	if (drumroll_playing)
	{
		music::stop(SE_DRUMROLL);
		drumroll_playing = false;
	}

	if (!score_finish_se_played)
	{
		music::play(SE_SCORE_FINISH, false);
		score_finish_se_played = true;
	}
}

//ボタンが押された瞬間トリガーを返す
bool is_confirm_triggered()
{
	if (TRG(0) & PAD_START)
		return true;

	return false;
}

//target まで duration フレームかけて到達するように計算（カウントアップ用）
int calc_count_up_by_time(int target, int& elapsed_frame, int duration)
{
	if (target <= 0)
	{
		return 0;
	}

	if (duration < 1)
	{
		duration = 1;
	}

	elapsed_frame++;
	if (elapsed_frame >= duration)
	{
		elapsed_frame = duration;
		return target;
	}

	float rate = (float)elapsed_frame / (float)duration;
	int value = (int)((float)target * rate);

	if (value > target)
	{
		value = target;
	}
	return value;
}

//start_val から target_val まで duration フレームかけて変化させる（カウントダウン対応）
int calc_count_move_by_time(int start_val, int target_val, int& elapsed_frame, int duration)
{
	if (start_val == target_val)
	{
		return target_val;
	}

	if (duration < 1)
	{
		duration = 1;
	}

	elapsed_frame++;
	if (elapsed_frame >= duration)
	{
		elapsed_frame = duration;
		return target_val;
	}

	float rate = (float)elapsed_frame / (float)duration;
	float moved = (float)start_val + (float)(target_val - start_val) * rate;
	int value = (int)moved;

	if (target_val < start_val)
	{
		if (value < target_val) value = target_val;
		if (value > start_val)  value = start_val;
	}
	else
	{
		if (value > target_val) value = target_val;
		if (value < start_val)  value = start_val;
	}

	return value;
}

//指定した桁数(digits)で数値を左から並べて描画
void draw_number(int value, int digits, int x, int y, float scale)
{
	if (digits < 1) digits = 1;
	if (digits > 5) digits = 5;

	int max_val = 1;
	for (int i = 0; i < digits; i++) max_val *= 10;
	if (value < 0) value = 0;
	if (value >= max_val) value = max_val - 1;

	int d[5] = { 0, 0, 0, 0, 0 };
	int tmp = value;
	for (int i = digits - 1; i >= 0; i--)
	{
		d[i] = tmp % 10;
		tmp /= 10;
	}

	for (int i = 0; i < digits; i++)
	{
		sprite_render(
			sprNumber,
			x + i * (score_W * scale), y,
			scale, scale,
			d[i] * score_W, 0,
			score_W, score_H
		);
	}
}

Sprite* get_rank_sprite(int target_score)
{
	if (difficulty == 0) // EASY
	{
		if (target_score >= 3000) return sprRankS;
		if (target_score >= 2200) return sprRankA;
		if (target_score >= 1500) return sprRankB;
		if (target_score >= 800)  return sprRankC;
		if (target_score >= 300)  return sprRankD;
		return sprRankE;
	}
	else if (difficulty == 1) // NORMAL
	{
		if (target_score >= 5000) return sprRankS;
		if (target_score >= 3800) return sprRankA;
		if (target_score >= 2600) return sprRankB;
		if (target_score >= 1500) return sprRankC;
		if (target_score >= 600)  return sprRankD;
		return sprRankE;
	}
	else if (difficulty == 2) // HARD
	{
		if (target_score >= 7000) return sprRankS;
		if (target_score >= 5200) return sprRankA;
		if (target_score >= 3600) return sprRankB;
		if (target_score >= 2000) return sprRankC;
		if (target_score >= 900)  return sprRankD;
		return sprRankE;
	}

	return sprRankE;
}

//hp_start を指定しない場合は 100 からカウントダウン開始
void result_score_set(int total_score, int kill_count, int level, int remain_hp)
{
	score = total_score;
	score_kill = kill_count;
	score_level = level;
	score_hp = remain_hp;
	start_hp = 100;

	disp_score = 0;
	disp_kill = 0;
	disp_level = 0;
	disp_hp = start_hp;

	count_frame_score = 0;
	count_frame_kill = 0;
	count_frame_level = 0;
	count_frame_hp = 0;

	active_item = 0;
	drumroll_playing = false;
	score_finish_se_played = false;
	total_score_calculated = false;
}

void result_score_init()
{
	result_score_state = 0;

	score = 0;
	score_kill = 0;
	score_level = 0;
	score_hp = 0;
	start_hp = 100;

	disp_score = 0;
	disp_kill = 0;
	disp_level = 0;
	disp_hp = 0;

	count_frame_score = 0;
	count_frame_kill = 0;
	count_frame_level = 0;
	count_frame_hp = 0;

	active_item = 0;

	score_W = 64;
	score_H = 64;

	drumroll_loaded = false;
	drumroll_playing = false;
	score_finish_se_played = false;
	total_score_calculated = false;
}

void result_score_deinit()
{
	if (drumroll_playing)
	{
		music::stop(SE_DRUMROLL);
		drumroll_playing = false;
	}

	safe_delete(sprNumber);
	safe_delete(sprRankS);
	safe_delete(sprRankA);
	safe_delete(sprRankB);
	safe_delete(sprRankC);
	safe_delete(sprRankD);
	safe_delete(sprRankE);
	safe_delete(sprResultText1);
	safe_delete(sprResultText2);
	safe_delete(sprResultText3);
}

void result_score_update()
{
	switch (result_score_state)
	{
	case 0:
		sprNumber = sprite_load(L"./Data/Images/number.png");

		sprRankS = sprite_load(L"./Data/Images/rankS.png");
		sprRankA = sprite_load(L"./Data/Images/rankA.png");
		sprRankB = sprite_load(L"./Data/Images/rankB.png");
		sprRankC = sprite_load(L"./Data/Images/rankC.png");
		sprRankD = sprite_load(L"./Data/Images/rankD.png");
		sprRankE = sprite_load(L"./Data/Images/rankE.png");

		sprResultText1 = sprite_load(L"./Data/Images/result_text1.png");
		sprResultText2 = sprite_load(L"./Data/Images/result_text2.png");
		sprResultText3 = sprite_load(L"./Data/Images/result_text3.png");

		if (!drumroll_loaded)
		{
			music::load(SE_DRUMROLL, L"./Data/Sounds/SE/drumRoll_start.wav");
			music::load(SE_SCORE_FINISH, L"./Data/Sounds/SE/drumRoll_end.wav");
			drumroll_loaded = true;
		}

		score = calc_total_score();
		total_score_calculated = true;

		result_score_state++;
		break;

	case 1:
	{
		bool pressed = is_confirm_triggered();

		// 0: キル数
		if (active_item == 0)
		{
			disp_kill = calc_count_up_by_time(score_kill, count_frame_kill, COUNT_DURATION);

			if (pressed)
			{
				disp_kill = score_kill;
				count_frame_kill = COUNT_DURATION;
				active_item = 1;
			}
			else if (disp_kill >= score_kill)
			{
				active_item = 1;
			}
		}
		// 1: タワーHP（start_hp から score_hp へカウントダウン）
		else if (active_item == 1)
		{
			disp_hp = calc_count_move_by_time(start_hp, score_hp, count_frame_hp, COUNT_DURATION);

			if (pressed)
			{
				disp_hp = score_hp;
				count_frame_hp = COUNT_DURATION;
				active_item = 2;
			}
			else if (count_frame_hp >= COUNT_DURATION)
			{
				disp_hp = score_hp;
				active_item = 2;
			}
		}
		// 2: レベル
		else if (active_item == 2)
		{
			disp_level = calc_count_up_by_time(score_level, count_frame_level, COUNT_DURATION);

			if (pressed)
			{
				disp_level = score_level;
				count_frame_level = COUNT_DURATION;
				active_item = 3;
			}
			else if (disp_level >= score_level)
			{
				active_item = 3;
			}
		}
		// 3: 総合スコア
		else if (active_item == 3)
		{
			if (!total_score_calculated)
			{
				score = calc_total_score();
				total_score_calculated = true;
			}

			//ドラムロール開始
			if (score > 0 && !drumroll_playing && !score_finish_se_played)
			{
				music::play(SE_DRUMROLL, true);
				drumroll_playing = true;
			}

			disp_score = calc_count_up_by_time(score, count_frame_score, COUNT_DURATION);

			if (pressed)
			{
				disp_score = score;
				count_frame_score = COUNT_DURATION;
				active_item = 4;
				finish_score_drumroll();
			}
			else if (score <= 0 || disp_score >= score)
			{
				disp_score = score;
				active_item = 4;
				finish_score_drumroll();
			}
		}
		break;
	}
	}
}

void result_score_render()
{
	if (result_score_state == 0) return;

	//1段目：倒した敵（左隣）＋ キル数（3桁）
	if (active_item >= 0)
	{
		if (sprResultText1 != nullptr)
		{
			sprite_render(sprResultText1, 320.0f, 195.0f, 1.0f, 1.0f);
		}
		draw_number(disp_kill, 3, 1020, 190, 1.7f);
	}

	//2段目：タワーHP（左隣）＋ タワーHP数値（3桁）
	if (active_item >= 1)
	{
		if (sprResultText2 != nullptr)
		{
			sprite_render(sprResultText2, 320.0f, 345.0f, 1.0f, 1.0f);
		}
		draw_number(disp_hp, 3, 1020, 340, 1.7f);
	}

	//3段目：レベル（左隣）＋ レベル数値（2桁）
	if (active_item >= 2)
	{
		if (sprResultText3 != nullptr)
		{
			sprite_render(sprResultText3, 320.0f, 495.0f, 1.0f, 1.0f);
		}
		draw_number(disp_level, 2, 1020, 490, 1.7f);
	}

	//4段目：総合スコア（5桁・少し大きめ）
	if (active_item >= 3)
	{
		draw_number(disp_score, 5, 250, 680, 2.3f);
	}

	//ランク描画（総合スコアが出始めたあとに表示）
	if (active_item >= 3)
	{
		Sprite* rank_spr = get_rank_sprite(disp_score);
		if (rank_spr != nullptr)
		{
			sprite_render(rank_spr, 1250, 680, 2.3f, 2.3f);
		}
	}
}