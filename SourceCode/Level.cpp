#include "Level.h"
#include "player.h"
#include "cursor_main.h"
#include "power_efficacy.h"

// 一円玉マシンガンのレベル性能設定
struct WEAPON_LEVEL_DATA
{
	float damage;       
	float cooltime;     
	float bullet_scale;
};
// 一円弾のレベル性能設定
const WEAPON_LEVEL_DATA Y1_LEVEL_DATA[8] =
{
	// ダメージ   クールタイム   攻撃範囲
	{ 100.000f,   100.000f,       100.000f }, // Lv1
	{ 136.500f,    98.409f,       103.020f }, // Lv2
	{ 202.800f,    95.519f,       108.202f }, // Lv3
	{ 318.565f,    90.473f,       115.672f }, // Lv4
	{ 514.008f,    81.951f,       125.562f }, // Lv5
	{ 835.409f,    67.946f,       138.010f }, // Lv6
	{1351.507f,    45.450f,       153.158f }, // Lv7
	{2164.824f,    10.000f,       171.154f }  // Lv8
};

// 貝殻ミサイルのレベル性能設定
const WEAPON_LEVEL_DATA SHELL_LEVEL_DATA[8] =
{
	// ダメージ   クールタイム   攻撃範囲
	{ 500.000f,   500.000f, 100.000f }, // Lv1
	{ 682.500f,   492.929f, 103.020f }, // Lv2
	{ 1014.400f,  480.685f, 108.202f }, // Lv3
	{ 1592.825f,  456.659f, 115.672f }, // Lv4
	{ 2570.490f,  419.780f, 125.562f }, // Lv5
	{ 4177.046f,  357.536f, 138.010f }, // Lv6
	{ 6757.533f,  257.557f, 153.158f }, // Lv7
	{ 10824.119f, 100.000f, 200.154f }  // Lv8
};
int level_up_weapon[WEAPON_MAX];

// インゴットのレベル性能設定
const WEAPON_LEVEL_DATA INGOT_LEVEL_DATA[8] =
{
	// ダメージ   クールタイム   弾のスケール
	{  100.000f, 300.000f,		50.000f	 }, // Lv1
	{  136.500f, 295.934f,		55.550f	 }, // Lv2
	{  202.800f, 288.549f,		62.920f	 }, // Lv3
	{  318.565f, 275.654f,		72.540f	 }, // Lv4
	{  514.098f, 253.874f,		84.918f	 }, // Lv5
	{  835.409f, 218.083f,		100.657f	 }, // Lv6
	{ 1351.507f, 160.595f,		120.466f	 }, // Lv7
	{ 2164.824f,  70.000f,		145.179f	 }  // Lv8
};

// ビームのレベル性能設定
const WEAPON_LEVEL_DATA BEAM_LEVEL_DATA[8] =
{
	// ダメージ   クールタイム   弾のスケール
	{  100.000f, 400.000f,		50.000f	 }, // Lv1
	{  136.500f, 395.581f,		55.550f	 }, // Lv2
	{  202.800f, 387.553f,		62.920f	 }, // Lv3
	{  318.565f, 373.537f,		72.540f	 }, // Lv4
	{  514.098f, 349.863f,		84.918f	 }, // Lv5
	{  835.409f, 310.960f,		100.657f }, // Lv6
	{ 1351.507f, 248.473f,		120.466f }, // Lv7
	{ 2164.824f, 150.000f,		145.179f }  // Lv8
};

// バンクロールのレベル性能設定
const WEAPON_LEVEL_DATA BANKROLL_LEVEL_DATA[8] =
{
	// ダメージ   クールタイム   弾のスケール
	{  30.000f, 300.000f,		50.000f	 }, // Lv1
	{  40.950f, 295.934f,		55.550f	 }, // Lv2
	{  60.840f, 288.549f,		62.920f	 }, // Lv3
	{  95.570f, 275.654f,		72.540f	 }, // Lv4
	{ 154.229f, 253.874f,		84.918f	 }, // Lv5
	{ 250.623f, 218.083f,		100.657f }, // Lv6
	{ 405.452f, 160.595f,		120.466f }, // Lv7
	{ 649.447f,  70.000f,		145.179f }  // Lv8
};

// 黄金ビームのレベル性能設定
//const WEAPON_LEVEL_DATA BANKROLL_LEVEL_DATA[8] =
//{
//	 フリーズ時間   クールタイム   攻撃範囲
//	{ 3.000f, 400.000f, 100.000f }, // Lv1
//	{ 3.245f, 393.813f, 103.020f }, // Lv2
//	{ 3.704f, 382.575f, 108.202f }, // Lv3
//	{ 4.411f, 362.952f, 115.672f }, // Lv4
//	{ 5.397f, 329.808f, 125.562f }, // Lv5
//	{ 6.700f, 275.344f, 138.010f }, // Lv6
//	{ 8.362f, 187.862f, 153.158f }, // Lv7
//	{ 10.427f, 500.000f, 171.154f } // Lv8
//};

void level_init()
{
	for (int i = 0; i < WEAPON_MAX; i++)
	{
		level_up_weapon[i] = 0;
	}
	// 最初から所持している武器
	level_up_weapon[Y1] = level_1;
}

void level_update()
{
	// 一円玉マシンガンのレベル
	int level = level_up_weapon[Y1];

	if (level < level_1)
	{
		level = level_1;
	}
	if (level > level_8)
	{
		level = level_8;
	}
	level_up_weapon[Y1] = level;
	const WEAPON_LEVEL_DATA& data = Y1_LEVEL_DATA[level - 1];
	// ダメージ
	Y1_damage = Y1_DAMAGE * data.damage / 100.0f;
	// クールタイム
	timer_down_bullet = static_cast<int>(TIMER * data.cooltime / 100.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
	// クールタイムの最低値
	if (timer_down_bullet < 1)
	{
		timer_down_bullet = 1;
	}
	// 弾のスケール
	bullet_scale = data.bullet_scale / 100.0f;

	// -----------------------------------------------------------
	// 
	// 貝殻ミサイルのレベル
	int shell_level = level_up_weapon[SHELL];

	// 取得済みの場合だけ性能を更新
	if (shell_level > 0)
	{
		if (shell_level > level_8)
		{
			shell_level = level_8;
		}

		level_up_weapon[SHELL] = shell_level;
		const WEAPON_LEVEL_DATA& shell_data = SHELL_LEVEL_DATA[shell_level - 1];
		// ダメージ
		shell_damage = SHELL_DAMAGE * shell_data.damage / 100.0f;
		// クールタイム
		timer_down_shell = static_cast<int>(TIMER_SHELL * shell_data.cooltime / 500.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
		// クールタイムの最低値
		if (timer_down_shell < 1)
		{
			timer_down_shell = 1;
		}
		// 弾のスケール
		shell_scale = 2.5f * shell_data.bullet_scale / 100.0f;
	}
	// -----------------------------------------------------------
	// インゴットのレベル

	int ingot_level = level_up_weapon[INGOT];

	if (ingot_level > 0)
	{
		if (ingot_level > level_8)
		{
			ingot_level = level_8;
		}

		level_up_weapon[INGOT] = ingot_level;
		const WEAPON_LEVEL_DATA& ingot_data = INGOT_LEVEL_DATA[ingot_level - 1];
		// ダメージ
		ingot_damage = INGOT_DAMAGE * ingot_data.damage / 100.0f;
		// クールタイム
		timet_down_ingot = static_cast<int>(TIMER_INGOT * ingot_data.cooltime / 100.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
		// クールタイムの最低値
		if (timet_down_ingot < 1)
		{
			timet_down_ingot = 1;
		}
		// 弾のスケール
		ingot_scale = 1.5f * ingot_data.bullet_scale / 100.0f;
	}

	// -----------------------------------------------------------
	// ビームのレベル

	int beam_level = level_up_weapon[BEAM];

	if (beam_level > 0)
	{
		if (beam_level > level_8)
		{
			beam_level = level_8;
		}

		level_up_weapon[BEAM] = beam_level;

		const WEAPON_LEVEL_DATA& beam_data = BEAM_LEVEL_DATA[beam_level - 1];

		// ダメージ
		beem_damage = BEEM_DAMAGE * beam_data.damage / 100.0f;
		// クールタイム
		timer_down_beam = static_cast<int>(TIMER_BEAM * beam_data.cooltime / 100.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
		// クールタイムの最低値
		if (timer_down_beam < 1)
		{
			timer_down_beam = 1;
		}
		// 弾のスケール
		beem_scale = 1.5f * beam_data.bullet_scale / 100.0f;
	}
	// -----------------------------------------------------------
	// バンクロールのレベル

	int bankroll_level = level_up_weapon[BANKROLL];

	if (bankroll_level > 0)
	{
		if (bankroll_level > level_8)
		{
			bankroll_level = level_8;
		}

		level_up_weapon[BANKROLL] = bankroll_level;

		const WEAPON_LEVEL_DATA& bankroll_data =
			BANKROLL_LEVEL_DATA[bankroll_level - 1];

		// ダメージ
		bankrool_damage = 20.0f * bankroll_data.damage / 100.0f;
		// クールタイム
		timer_down_bankroll = static_cast<int>(TIMER_BANKROLL * bankroll_data.cooltime / 100.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
		// クールタイムの最低値
		if (timer_down_bankroll < 1)
		{
			timer_down_bankroll = 1;
		}
		// 弾のスケール
		bankrool_scale = bankroll_data.bullet_scale / 100.0f;
	}
	// -----------------------------------------------------------
	// 黄金ビームのレベル

	//int special_level = level_up_weapon[WEAPON_MAX];

	//if (special_level > 0)
	//{
	//	if (special_level > level_8)
	//	{
	//		special_level = level_8;
	//	}

	//	level_up_weapon[WEAPON_MAX] = special_level;

	//	const WEAPON_LEVEL_DATA& special_data =
	//		BANKROLL_LEVEL_DATA[special_level - 1];

	//	// ダメージ
	//	bankrool_damage = 20.0f * special_data.damage / 100.0f;
	//	// クールタイム
	//	timer_down_bankroll = static_cast<int>(TIMER_BANKROLL * special_data.cooltime / 100.0f * red_cooltime_rate * bullet_interval_rate + 0.5f);
	//	// クールタイムの最低値
	//	if (timer_down_bankroll < 1)
	//	{
	//		timer_down_bankroll = 1;
	//	}
	//	// 弾のスケール
	//	bankrool_scale = special_data.bullet_scale / 100.0f;
	//}
}