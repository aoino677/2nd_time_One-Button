#pragma once

#pragma once
#include "all.h"

#pragma once

void result_score_init();
void result_score_deinit();
void result_score_update();
void result_score_render();

// 総合スコア・キル数・レベル・残りHPを受け取る
void result_score_set(int total_score, int kill_count, int level, int remain_hp);

// 他ファイルから参照・設定するスコア変数
extern int score;
extern int score_hp;
extern int score_kill;
