#pragma once

#define PARTICLE_NUMBER 150 

void particles_init();
void particles_update(float x, float y, int s, float r, float g, float b);
void particle_render();

//死亡エフェクト構造体
struct PARTICLE {
	bool active;      //使用中かどうか
	float x, y;       //座標
	float vx, vy;     //速度
	float size;       //ブロックの大きさ
	float alpha;      //透明度
	int life;         //残り寿命
	int max_life;     //全体の寿命
	float r, g, b;    //色
};