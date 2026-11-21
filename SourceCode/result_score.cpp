#include "result_score.h"

Sprite* sprNumber;

Sprite* sprRankS;
Sprite* sprRankA;
Sprite* sprRankB;
Sprite* sprRankC;
Sprite* sprRankD;
Sprite* sprRankE;

int result_score_state;

int score;
int score_hp;
int score_kill;

int score_W;
int score_H;

int one;
int ten;
int hundred;
int thousand;
int ten_thousand;

// 数値を5桁に分解するだけのシンプルな関数
void calc_digits(int value)
{
    if (value < 0)     value = 0;
    if (value > 99999) value = 99999;

    ten_thousand = (value / 10000) % 10;
    thousand = (value / 1000) % 10;
    hundred = (value / 100) % 10;
    ten = (value / 10) % 10;
    one = value % 10;
}

// x, y の位置から5桁を左から並べて描画
void draw_number_5(int x, int y)
{
    int d[5] = { ten_thousand, thousand, hundred, ten, one };

    for (int i = 0; i < 5; i++)
    {
        sprite_render(sprNumber, x + i * score_W, y, score_W, score_H, d[i] * score_W, 0, score_W, score_H);
    }
}

void result_score_init()
{
    result_score_state = 0;

    score = 0;
    score_hp = 0;
    score_kill = 0;

    score_W = 32;
    score_H = 32;

    one = 0;
    ten = 0;
    hundred = 0;
    thousand = 0;
    ten_thousand = 0;
}

void result_score_deinit()
{
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

        result_score_state = 1;
        break;

    case 1:
        // 倒した敵スコア・タワーHPスコア・総合スコアの計算
        score = score_kill + score_hp;
        calc_digits(score);
        break;
    }
}

void result_score_render()
{
    if (result_score_state == 0) return;

    // 総合スコアを5桁描画
    draw_number_5(600, 300);
}