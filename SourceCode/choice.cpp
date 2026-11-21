#include "choice.h"
#include "level_up.h"
#include "power_efficacy.h"
#include "player.h"
#include "cursor_main.h"
#include "Level.h"
#include <ctime>
#include <stdlib.h>

OBJ2D choiceButton[CHOICE_MAX];
Sprite* sprchoicebar;
OBJ2D power_efficacy[EFFICACY_MAX]; // 効能の個数
Sprite* sprpower_efficacy[EFFICACY_MAX];
OBJ2D levelup;
Sprite* sprlevelup;

int choice_state = 0;
int choice_timer = 0;
int level_up_count = 0;
int random[3];
float choice_effect_y = 0.0f;
float choice_effect_alpha = 0.0f;
float level_text_scale[CHOICE_MAX] = { 100.0f, 100.0f, 100.0f };
float efficacy_scale[CHOICE_MAX] = { 1.0f, 1.0f, 1.0f };
int choice_count = CHOICE_MAX;

void choice_init()
{
	choice_state = 0;
	choice_timer = 0;
	level_up_count = 0;
	bullet_lay_count = 1;
	choice_effect_y = 0.0f;
	choice_effect_alpha = 0.0f;

	for (int i = 0; i < CHOICE_MAX; i++)
	{
		efficacy_scale[i] = 1.0f;
	}

	// choiceButtonの読み込み
	sprchoicebar = sprite_load(L"./Data/Images/card_frame.png");
	// 文の読み込み
	sprpower_efficacy[0] = sprite_load(L"./Data/images/card_hp.png");
	sprpower_efficacy[1] = sprite_load(L"./Data/images/card_shell.png");
	sprpower_efficacy[2] = sprite_load(L"./Data/images/card_issen.png");
	sprpower_efficacy[3] = sprite_load(L"./Data/images/card_fall.png");
	sprpower_efficacy[4] = sprite_load(L"./Data/images/card_bankroll.png");
	sprpower_efficacy[5] = sprite_load(L"./Data/images/card_lucky.png");
	sprpower_efficacy[6] = sprite_load(L"./Data/images/card_rifle.png");
	sprpower_efficacy[7] = sprite_load(L"./Data/images/card_attack.png");
	sprpower_efficacy[8] = sprite_load(L"./Data/images/card_range.png");
	sprpower_efficacy[9] = sprite_load(L"./Data/images/card_rapid.png");
	sprpower_efficacy[10] = sprite_load(L"./Data/images/card_knokback.png");
	sprpower_efficacy[11] = sprite_load(L"./Data/images/card_score.png");
	// 「レベルアップ！」の画像読み込み
	sprlevelup = sprite_load(L"./Data/Images/levelUP.png");

	// パラメーターの設定
	for (int i = 0; i < CHOICE_MAX; i++)
	{
		choiceButton[i] = {};
		choiceButton[0].pos		= { 450.0f, BUTTON_Y };
		choiceButton[1].pos		= { 950.0f, BUTTON_Y };
		choiceButton[2].pos		= { 1450.0f, BUTTON_Y };
		choiceButton[i].scale		= { static_cast<float>(CHOICE_SCALE)    , static_cast<float>(CHOICE_SCALE) };
		choiceButton[i].texSize	= { static_cast<float>(CHOICE_TEX_W)    , static_cast<float>(CHOICE_TEX_H) };
		choiceButton[i].pivot		= { static_cast<float>(CHOICE_PIVOT_X)  ,static_cast<float>(CHOICE_PIVOT_Y) };
		choiceButton[i].color		= { 1.0f, 1.0f, 1.0f, 1.0f };
	}
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		power_efficacy[i] = {};
		power_efficacy[i].scale		= { static_cast<float>(POWER_EFFICACY_SCALE) ,static_cast<float>(POWER_EFFICACY_SCALE) };
		power_efficacy[i].texSize		= { static_cast<float>(POWER_EFFICACY_TEX_W) , static_cast<float>(POWER_EFFICACY_TEX_H) };
		power_efficacy[i].pivot		= { static_cast<float>(POWER_EFFICACY_PIVOT_X)  ,static_cast<float>(POWER_EFFICACY_PIVOT_Y) };
		power_efficacy[i].color		= { 1, 1, 1, 0.0f };
	}
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		power_efficacy[i].pos = choiceButton[0].pos;
		power_efficacy[i].pos = choiceButton[1].pos;
		power_efficacy[i].pos = choiceButton[2].pos;
	}
	levelup = {};
	levelup.pos			= { LEVELUP_POS_X , LEVELUP_POS_Y };
	levelup.scale		= { static_cast<float>(LEVELUP_SCALE), static_cast<float>(LEVELUP_SCALE) };
	levelup.texSize		= { static_cast<float>(LEVELUP_TEX_W), static_cast<float>(LEVELUP_TEX_H) };
	levelup.pivot		= { static_cast<float>(LEVELUP_PIVOT_X), static_cast<float>(LEVELUP_PIVOT_Y) };
	levelup.color		= { 1.0f, 1.0f, 1.0f, 1.0f };
}

void choice_update_pause()
{
	if (!choice_pause)
	{
		if (exp_count >= level_max)
		{
			level_up_count++;
			for (int i = 1; i <= level_up_count; i++)
			{
				for (; T_or_F(level_up_count);)
				{
					exp_count -= level_max;
					level_max += LEVEL_COUNT_INCREAS;
					choice_pause = true;
					choice_effect_y = static_cast<float>(SCREEN_H);
					choice_effect_alpha = 0.0f;
					choice_timer = 0;
					choice_state = 0;
					level_up_count--;

					randoms();
				}
				if (i == level_up_count)
				{
					level_up_count = 0;
				}
			}
		}
	}
}

void choice_update()
{
	switch (choice_state)
	{
	case 0:
		//----------------------------------
		// 演出開始
		//----------------------------------
		choice_effect_y = static_cast<float>(SCREEN_H);
		choice_effect_alpha = 0.0f;
		choice_timer = 0;

		choice_state++;
		break;
	case 1:
		choice_timer++;

		// 背景を徐々に暗くする
		choice_effect_alpha += 0.04f;

		if (choice_effect_alpha >= 0.9f)
		{
			choice_effect_alpha = 0.9f;
		}

		// ボタンを下から上へ移動
		choice_effect_y -= 40.0f;

		if (choice_effect_y <= 0.0f)
		{
			choice_effect_y = 0.0f;
		}

		// 演出終了
		if (choice_effect_y <= 0.0f &&
			choice_effect_alpha >= 0.8f)
		{
			choice_state++;
		}
		break;

	case 2:
		// ランダムで画像を表示
		switch (random[0])
		{
		case HP_UP: // プレイヤーのHPが上がる
			power_efficacy[0].pos = { 450,BUTTON_Y };
			power_efficacy[0].color.w = 1;break;

		case WEAPON_SHELL: // ミサイルを飛ばす
			power_efficacy[1].pos = { 450,BUTTON_Y };
			power_efficacy[1].color.w = 1;break;

		case WEAPON_BEAM: // ビームを飛ばす
			power_efficacy[2].pos = { 450,BUTTON_Y };
			power_efficacy[2].color.w = 1;break;

		case WEAPON_INGOT: // インゴットを飛ばす
			power_efficacy[3].pos = { 450,BUTTON_Y };
			power_efficacy[3].color.w = 1;break;

		case WEAPON_BANKROLL: // お札でたたく
			power_efficacy[4].pos = { 450,BUTTON_Y };
			power_efficacy[4].color.w = 1;break;

		case ITEM_LUCKY: // アイテムが落ちやすくなる
			power_efficacy[5].pos = { 450,BUTTON_Y };
			power_efficacy[5].color.w = 1;break;

		case WEAPON_BULLET: // 一円玉を発射する
			power_efficacy[6].pos = { 450,BUTTON_Y };
			power_efficacy[6].color.w = 1;break;

		case BULLET_DAMAGE: // 弾のダメージが上がる
			power_efficacy[7].pos = { 450,BUTTON_Y };
			power_efficacy[7].color.w = 1;break;

		case CURSOR_BIG: // カーソルを大きくする
			power_efficacy[8].pos = { 450,BUTTON_Y };
			power_efficacy[8].color.w = 1; break;

		case BULLET_INTERVAL: // 弾の間隔を短くする
			power_efficacy[9].pos = { 450,BUTTON_Y };
			power_efficacy[9].color.w = 1; break;

		case HIT_BUCK: // ヒットバックする距離を増やす
			power_efficacy[10].pos = { 450,BUTTON_Y };
			power_efficacy[10].color.w = 1; break;

		case SCORE: // スコアを増やす
			power_efficacy[11].pos = { 450,BUTTON_Y };
			power_efficacy[11].color.w = 1; break;
		}
		switch (random[1])
		{
		case HP_UP: // プレイヤーのHPが上がる
			power_efficacy[0].pos = { 950,BUTTON_Y };
			power_efficacy[0].color.w = 1;break;

		case WEAPON_SHELL: // ミサイルを飛ばす
			power_efficacy[1].pos = { 950,BUTTON_Y };
			power_efficacy[1].color.w = 1;break;

		case WEAPON_BEAM: // ビームを飛ばす
			power_efficacy[2].pos = { 950,BUTTON_Y };
			power_efficacy[2].color.w = 1;break;

		case WEAPON_INGOT: // インゴットを飛ばす
			power_efficacy[3].pos = { 950,BUTTON_Y };
			power_efficacy[3].color.w = 1;break;

		case WEAPON_BANKROLL: // お札でたたく
			power_efficacy[4].pos = { 950,BUTTON_Y };
			power_efficacy[4].color.w = 1;break;

		case ITEM_LUCKY: // アイテムが落ちやすくなる
			power_efficacy[5].pos = { 950,BUTTON_Y };
			power_efficacy[5].color.w = 1;break;
		
		case WEAPON_BULLET: // 一円玉を発射する
			power_efficacy[6].pos = { 950,BUTTON_Y };
			power_efficacy[6].color.w = 1;break;

		case BULLET_DAMAGE: // 弾のダメージが上がる
			power_efficacy[7].pos = { 950,BUTTON_Y };
			power_efficacy[7].color.w = 1;break;

		case CURSOR_BIG: // カーソルを大きくする
			power_efficacy[8].pos = { 950,BUTTON_Y };
			power_efficacy[8].color.w = 1; break;

		case BULLET_INTERVAL: // 弾の間隔を短くする
			power_efficacy[9].pos = { 950,BUTTON_Y };
			power_efficacy[9].color.w = 1; break;

		case HIT_BUCK: // ヒットバックする距離を増やす
			power_efficacy[10].pos = { 950,BUTTON_Y };
			power_efficacy[10].color.w = 1; break;

		case SCORE: // スコアを増やす
			power_efficacy[11].pos = { 950,BUTTON_Y };
			power_efficacy[11].color.w = 1; break;
		}
		switch (random[2])
		{
		case HP_UP: // プレイヤーのHPが上がる
			power_efficacy[0].pos = { 1450,BUTTON_Y };
			power_efficacy[0].color.w = 1;break;

		case WEAPON_SHELL: // ミサイルを飛ばす
			power_efficacy[1].pos = { 1450,BUTTON_Y };
			power_efficacy[1].color.w = 1;break;

		case WEAPON_BEAM: // ビームを飛ばす
			power_efficacy[2].pos = { 1450,BUTTON_Y };
			power_efficacy[2].color.w = 1;break;

		case WEAPON_INGOT: // インゴットを飛ばす
			power_efficacy[3].pos = { 1450,BUTTON_Y };
			power_efficacy[3].color.w = 1;break;

		case WEAPON_BANKROLL: // お札でたたく
			power_efficacy[4].pos = { 1450,BUTTON_Y };
			power_efficacy[4].color.w = 1;break;

		case ITEM_LUCKY: // アイテムが落ちやすくなる
			power_efficacy[5].pos = { 1450,BUTTON_Y };
			power_efficacy[5].color.w = 1;break;
		
		case WEAPON_BULLET: // 一円玉を発射する
			power_efficacy[6].pos = { 1450,BUTTON_Y };
			power_efficacy[6].color.w = 1;break;

		case BULLET_DAMAGE: // 弾のダメージが上がる
			power_efficacy[7].pos = { 1450,BUTTON_Y };
			power_efficacy[7].color.w = 1;break;

		case CURSOR_BIG: // カーソルを大きくする
			power_efficacy[8].pos = { 1450,BUTTON_Y };
			power_efficacy[8].color.w = 1; break;

		case BULLET_INTERVAL: // 弾の間隔を短くする
			power_efficacy[9].pos = { 1450,BUTTON_Y };
			power_efficacy[9].color.w = 1; break;

		case HIT_BUCK: // ヒットバックする距離を増やす
			power_efficacy[10].pos = { 1450,BUTTON_Y };
			power_efficacy[10].color.w = 1; break;

		case SCORE: // スコアを増やす
			power_efficacy[11].pos = { 1450,BUTTON_Y };
			power_efficacy[11].color.w = 1; break;
		}
		for (int i = 0; i < choice_count; i++)
		{
			float mouseX = static_cast<float>(getCursorPosX());
			float mouseY = static_cast<float>(getCursorPosY());

			float buttonY = choiceButton[i].pos.y + choice_effect_y;
			float left = choiceButton[i].pos.x - choiceButton[i].pivot.x * choiceButton[i].scale.x;
			float right = choiceButton[i].pos.x + (choiceButton[i].texSize.x - choiceButton[i].pivot.x) * choiceButton[i].scale.x;
			float top = buttonY - choiceButton[i].pivot.y * choiceButton[i].scale.y;
			float bottom = buttonY + (choiceButton[i].texSize.y - choiceButton[i].pivot.y) * choiceButton[i].scale.y;

			
			
			//----------------------------------
			// ポーズボタンの判定
			//----------------------------------
			if (mouseX >= left && mouseX <= right &&
				mouseY >= top && mouseY <= bottom)
			{
				choiceButton[i].scale.x = CHOICE_SCALE + 0.1f;
				choiceButton[i].scale.y = CHOICE_SCALE + 0.1f;
				efficacy_scale[i] = POWER_EFFICACY_SCALE + 0.1f;
				// SCORE以外は従来通り拡大
				if (random[i] != SCORE)
				{
					power_efficacy[random[i]].scale.x = POWER_EFFICACY_SCALE + 0.1f;
					power_efficacy[random[i]].scale.y = POWER_EFFICACY_SCALE + 0.1f;
				}
				level_text_scale[i] = 1.1f;
				if (TRG(0) & PAD_START)
				{
					switch (i)
					{
					case 0:
						choice_pause = false;
						choice_state = 0;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (random[0])
						{
						case HP_UP:
							HP_Max();
							power_efficacy[random[i]].color.w = 0 ;break;
						case WEAPON_SHELL:
							if (level_up_weapon[SHELL] == 0)	{level_up_weapon[SHELL] = level_1;}
							else if (level_up_weapon[SHELL] < level_8){level_up_weapon[SHELL]++;}
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BEAM:
							if (level_up_weapon[BEAM] == 0) { level_up_weapon[BEAM] = level_1; }
							else if (level_up_weapon[BEAM] < level_8) { level_up_weapon[BEAM]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_INGOT:
							if (level_up_weapon[INGOT] == 0) { level_up_weapon[INGOT] = level_1; }
							else if (level_up_weapon[INGOT] < level_8) { level_up_weapon[INGOT]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BANKROLL:
							if (level_up_weapon[BANKROLL] == 0) { level_up_weapon[BANKROLL] = level_1; }
							else if (level_up_weapon[BANKROLL] < level_8) { level_up_weapon[BANKROLL]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case ITEM_LUCKY:
							Bullet_Lucky();			
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BULLET:
							if (level_up_weapon[Y1] < level_8) { level_up_weapon[Y1]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_DAMAGE:
							Bullet_Damage();		
							power_efficacy[random[i]].color.w = 0; break;
						case CURSOR_BIG:
							Cursor_big();
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_INTERVAL:
							Bullet_Interval();
							power_efficacy[random[i]].color.w = 0; break;
						case HIT_BUCK:
							Hit_Bag();
							power_efficacy[random[i]].color.w = 0; break;
						case SCORE:
							Score_Up();
							power_efficacy[random[i]].color.w = 0; break;
						}
						break;

					case 1:
						choice_pause = false;
						choice_state = 0;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (random[1])
						{
						case HP_UP:
							HP_Max();
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_SHELL:
							if (level_up_weapon[SHELL] == 0) { level_up_weapon[SHELL] = level_1; }
							else if (level_up_weapon[SHELL] < level_8) { level_up_weapon[SHELL]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BEAM:
							if (level_up_weapon[BEAM] == 0) { level_up_weapon[BEAM] = level_1; }
							else if (level_up_weapon[BEAM] < level_8) { level_up_weapon[BEAM]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_INGOT:
							if (level_up_weapon[INGOT] == 0) { level_up_weapon[INGOT] = level_1; }
							else if (level_up_weapon[INGOT] < level_8) { level_up_weapon[INGOT]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BANKROLL:
							if (level_up_weapon[BANKROLL] == 0) { level_up_weapon[BANKROLL] = level_1; }
							else if (level_up_weapon[BANKROLL] < level_8) { level_up_weapon[BANKROLL]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case ITEM_LUCKY:
							Bullet_Lucky();
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BULLET:
							if (level_up_weapon[Y1] < level_8) { level_up_weapon[Y1]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_DAMAGE:
							Bullet_Damage();
							power_efficacy[random[i]].color.w = 0; break;
						case CURSOR_BIG:
							Cursor_big();
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_INTERVAL:
							Bullet_Interval();
							power_efficacy[random[i]].color.w = 0; break;
						case HIT_BUCK:
							Hit_Bag();
							power_efficacy[random[i]].color.w = 0; break;
						case SCORE:
							Score_Up();
							power_efficacy[random[i]].color.w = 0; break;
						}
						break;

					case 2:
						choice_pause = false;
						choice_state = 0;
						choice_effect_y = 0.0f;
						choice_effect_alpha = 0.0f;
						// ここに強化する効果を書く
						switch (random[2])
						{
						case HP_UP:
							HP_Max();
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_SHELL:
							if (level_up_weapon[SHELL] == 0) { level_up_weapon[SHELL] = level_1; }
							else if (level_up_weapon[SHELL] < level_8) { level_up_weapon[SHELL]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BEAM:
							if (level_up_weapon[BEAM] == 0) { level_up_weapon[BEAM] = level_1; }
							else if (level_up_weapon[BEAM] < level_8) { level_up_weapon[BEAM]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_INGOT:
							if (level_up_weapon[INGOT] == 0) { level_up_weapon[INGOT] = level_1; }
							else if (level_up_weapon[INGOT] < level_8) { level_up_weapon[INGOT]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BANKROLL:
							if (level_up_weapon[BANKROLL] == 0) { level_up_weapon[BANKROLL] = level_1; }
							else if (level_up_weapon[BANKROLL] < level_8) { level_up_weapon[BANKROLL]++; }
							power_efficacy[random[i]].color.w = 0; break;
						case ITEM_LUCKY:
							Bullet_Lucky();
							power_efficacy[random[i]].color.w = 0; break;
						case WEAPON_BULLET:
							if (level_up_weapon[Y1] < level_8){level_up_weapon[Y1]++;}
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_DAMAGE:
							Bullet_Damage();
							power_efficacy[random[i]].color.w = 0; break;
						case CURSOR_BIG:
							Cursor_big();
							power_efficacy[random[i]].color.w = 0; break;
						case BULLET_INTERVAL:
							Bullet_Interval();
							power_efficacy[random[i]].color.w = 0; break;
						case HIT_BUCK:
							Hit_Bag();
							power_efficacy[random[i]].color.w = 0; break;
						case SCORE:
							Score_Up();
							power_efficacy[random[i]].color.w = 0; break;
						}
						break;
					}
				}
			}
			else
			{
				choiceButton[i].scale.x = CHOICE_SCALE;
				choiceButton[i].scale.y = CHOICE_SCALE;
				efficacy_scale[i] = POWER_EFFICACY_SCALE;
				// SCORE以外は従来通り元の大きさに戻す
				if (random[i] != SCORE)
				{
					power_efficacy[random[i]].scale.x = POWER_EFFICACY_SCALE;
					power_efficacy[random[i]].scale.y = POWER_EFFICACY_SCALE;
				}
				level_text_scale[i] = 1.0f;
			}
		}
		break;
	}
}

void choice_render()
{
	if (choice_pause == true)
	{
	//----------------------------------
   // 暗い背景
   //----------------------------------
		primitive::rect(
			0.0f, 0.0f,
			static_cast<float>(SCREEN_W),
			static_cast<float>(SCREEN_H),
			0.0f, 0.0f,
			0.0f,
			1.00f, 0.78f, 0.55f,
			choice_effect_alpha
		);
		for (int i = 0; i < choice_count; i++)
		{
			sprite_render(
				sprchoicebar,
				choiceButton[i].pos.x, choiceButton[i].pos.y + choice_effect_y,
				choiceButton[i].scale.x, choiceButton[i].scale.y,
				0, 0,
				choiceButton[i].texSize.x, choiceButton[i].texSize.y,
				choiceButton[i].pivot.x, choiceButton[i].pivot.y,
				ToRadian(0),
				1, 1, 1, 1
			);
		}
		for (int i = 0; i < EFFICACY_MAX; i++)
		{
			// SCOREは下でカードごとに描画する
			if (i == SCORE)
			{
				continue;
			}

			sprite_render(
				sprpower_efficacy[i],
				power_efficacy[i].pos.x, power_efficacy[i].pos.y + choice_effect_y,
				power_efficacy[i].scale.x, power_efficacy[i].scale.y,
				0, 0,
				power_efficacy[i].texSize.x, power_efficacy[i].texSize.y,
				power_efficacy[i].pivot.x, power_efficacy[i].pivot.y,
				ToRadian(0),
				1, 1, 1, power_efficacy[i].color.w
			);
		}

		// スコアアップはカードごとに描画する
		for (int i = 0; i < choice_count; i++)
		{
			if (random[i] == SCORE)
			{
				sprite_render(
					sprpower_efficacy[SCORE],
					choiceButton[i].pos.x, choiceButton[i].pos.y + choice_effect_y,
					efficacy_scale[i], efficacy_scale[i],
					0, 0,
					power_efficacy[SCORE].texSize.x, power_efficacy[SCORE].texSize.y,
					power_efficacy[SCORE].pivot.x, power_efficacy[SCORE].pivot.y,
					ToRadian(0),
					1, 1, 1, power_efficacy[SCORE].color.w
				);
			}
		}
		sprite_render(
			sprlevelup,
			levelup.pos.x, levelup.pos.y + choice_effect_y,
			levelup.scale.x, levelup.scale.y,
			0, 0,
			levelup.texSize.x, levelup.texSize.y,
			levelup.pivot.x, levelup.pivot.y,
			ToRadian(0),
			1.0f, 1.0f, 1.0f, 1.0f
		);
	}
	if (choice_pause)
	{
		for (int i = 0; i < choice_count; i++)
		{
			DrawLevelText(
				random[i],
				choiceButton[i].pos.x - 60,
				choiceButton[i].pos.y + 240 + choice_effect_y,
				level_text_scale[i]
			);
		}
	}

}

void choice_deinit()
{
	safe_delete(sprchoicebar);
	safe_delete(sprlevelup);
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		safe_delete(sprpower_efficacy[i]);
	}
}

bool T_or_F(int level_up_count)
{
	if (level_up_count == 0) { return false; }
	else { return true; }
}

void randoms()
{
	int available[EFFICACY_MAX];
	int available_count = 0;

	// 強化できるものを調べる
	for (int i = 0; i < EFFICACY_MAX - 1; i++)
	{
		bool can_level_up = false;

		switch (i)
		{
		case HP_UP:
			can_level_up = hp_level < level_5;
			break;

		case WEAPON_SHELL:
			can_level_up = level_up_weapon[SHELL] < level_8;
			break;

		case WEAPON_BEAM:
			can_level_up = level_up_weapon[BEAM] < level_8;
			break;

		case WEAPON_INGOT:
			can_level_up = level_up_weapon[INGOT] < level_8;
			break;

		case WEAPON_BANKROLL:
			can_level_up = level_up_weapon[BANKROLL] < level_8;
			break;

		case ITEM_LUCKY:
			can_level_up = lucky_level < 16;
			break;

		case WEAPON_BULLET:
			can_level_up = level_up_weapon[Y1] < level_8;
			break;

		case BULLET_DAMAGE:
			can_level_up = bullet_damage_level < level_7;
			break;

		case CURSOR_BIG:
			can_level_up = cursor_level < level_8;
			break;

		case BULLET_INTERVAL:
			can_level_up = bullet_interval_level < level_7;
			break;

		case HIT_BUCK:
			can_level_up = hit_buck_level < level_5;
			break;
		}

		// 強化できるものだけ登録
		if (can_level_up)
		{
			available[available_count] = i;
			available_count++;
		}
	}

	// 常に3枚表示
	choice_count = CHOICE_MAX;

	// 強化候補をランダムに選ぶ
	int select_count = available_count;

	if (select_count > CHOICE_MAX)
	{
		select_count = CHOICE_MAX;
	}

	for (int i = 0; i < select_count; i++)
	{
		int index = rand() % available_count;

		random[i] = available[index];

		// 選んだ候補を削除して重複を防ぐ
		available[index] = available[available_count - 1];
		available_count--;
	}

	// 足りない分はスコアアップにする
	for (int i = select_count; i < CHOICE_MAX; i++)
	{
		random[i] = SCORE;
	}

	// すべての画像を非表示にする
	for (int i = 0; i < EFFICACY_MAX; i++)
	{
		power_efficacy[i].color.w = 0.0f;
	}
}
void DrawLevelText(int type, float x, float y, float scale)
{
	char str[64];

	int lv = 0;
	int maxLv = level_8;

	switch (type)
	{
	case HP_UP:
		lv = hp_level;
		maxLv = level_5;
		break;

	case WEAPON_SHELL:
		lv = level_up_weapon[SHELL];
		maxLv = level_8;
		break;

	case WEAPON_BEAM:
		lv = level_up_weapon[BEAM];
		maxLv = level_8;
		break;

	case WEAPON_INGOT:
		lv = level_up_weapon[INGOT];
		maxLv = level_8;
		break;

	case WEAPON_BANKROLL:
		lv = level_up_weapon[BANKROLL];
		maxLv = level_8;
		break;

	case ITEM_LUCKY:
		lv = lucky_level;
		maxLv = 16;
		break;

	case WEAPON_BULLET:
		if (level_up_weapon[Y1] == 0)
		{
			lv = 1;
		}
		else
		{
			lv = level_up_weapon[Y1];
		}
		maxLv = level_8;
		break;

	case BULLET_DAMAGE:
		lv = bullet_damage_level;
		maxLv = level_7;
		break;

	case CURSOR_BIG:
		lv = cursor_level;
		maxLv = level_8;
		break;

	case BULLET_INTERVAL:
		lv = bullet_interval_level;
		maxLv = level_7;
		break;

	case HIT_BUCK:
		lv = hit_buck_level;
		maxLv = level_5;
		break;
	}

	// 最大レベルならMAXを表示
	if (lv >= maxLv)
	{
		sprintf_s(str, "MAX");
	}
	else
	{
		sprintf_s(str, "%d   %d", lv, lv + 1);
	}

	font::textOut(
		6, str,
		x, y,
		scale, scale,
		1.0f, 0.8f, 0.2f, 1.0f,
		TEXT_ALIGN::UPPER_LEFT,
		false
	);
}