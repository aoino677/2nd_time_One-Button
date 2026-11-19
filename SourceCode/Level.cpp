#include "Level.h"
#include "player.h"
#include "cursor_main.h"

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
	{ 5000.000f,   100.000f,       100.000f }, // Lv1
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
	{ 10824.119f, 100.000f, 171.154f }  // Lv8
};
int level_up_weapon[WEAPON_MAX];


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
	timer_down = static_cast<int>(TIMER * data.cooltime / 100.0f + 0.5f);
	// クールタイムの最低値
	if (timer_down < 1)
	{
		timer_down = 1;
	}
	// 弾のスケール
	bullet_scale = data.bullet_scale / 100.0f;

	// -----------------------------------------------------------
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
		shell_damage = SHELL_DAMAGE * shell_data.damage / 500.0f;

		// 弾のスケール
		shell_scale = 2.5f * shell_data.bullet_scale / 100.0f;
	}
}