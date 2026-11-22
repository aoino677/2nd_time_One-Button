#pragma once
#include "all.h"

// 共通の初期化・終了処理
void scene_wipe_init();
void scene_wipe_deinit();

// 画面を閉じる演出（シーン切り替え前）
void scene_wipe_in_start();
void scene_wipe_in_update();
void scene_wipe_in_render();
bool is_scene_wipe_in_finished(); // 閉じきったら true

// 画面を開く演出（新しいシーンが始まった直後）
void scene_wipe_out_start();
void scene_wipe_out_update();
void scene_wipe_out_render();
bool is_scene_wipe_out_finished(); // 開ききったら true
