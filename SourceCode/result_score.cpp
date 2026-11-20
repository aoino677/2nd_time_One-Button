#include"result_score.h"

Sprete* sprNumber;

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


void result_score_init()
{
	score = 0;
	score_hp = 0;
	score_kill = 0;

    score_W = 32;
    score_H = 32;
    currentScore = score;
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
        
        sprRankS=sprite_load(L"./Data/Images/rankS.png");
        sprRankA=sprite_load(L"./Data/Images/rankA.png");
        sprRankB=sprite_load(L"./Data/Images/rankB.png");
        sprRankC=sprite_load(L"./Data/Images/rankC.png");
        sprRankD=sprite_load(L"./Data/Images/rankD.png");
        sprRankE=sprite_load(L"./Data/Images/rankE.png");
        
        result_score_state++;

    case 1:
        //倒した敵



        //タワーHP


        //総合スコア


    }

    

}

void result_score_render()
{

}