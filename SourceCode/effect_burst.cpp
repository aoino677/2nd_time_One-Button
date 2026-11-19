#include "all.h"
#include "effect_burst.h"

PARTICLE particle[PARTICLE_NUMBER];

void particles_init()
{
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		particle[i].active = false;
	}
}

//エフェクト(X座標,Y座標,大きさ,赤,緑,青)
void particles_update(float x, float y, int s, float r, float g, float b)
{
	int spawn_count = 12;
	int created = 0;

	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (!particle[i].active)
		{
			particle[i].active = true;
			particle[i].x = x;
			particle[i].y = y;

			float angle = (float)(rand() % 360) * 3.14159f / 180.0f;
			float speed = (float)(rand() % 8 + 2);
			particle[i].vx = cosf(angle) * speed;
			particle[i].vy = sinf(angle) * speed;

			particle[i].size = (float)(rand() % 8 + s);
			particle[i].max_life = rand() % 20 + 20;
			particle[i].life = particle[i].max_life;
			particle[i].alpha = 1.0f;

			particle[i].r = r;
			particle[i].g = g;
			particle[i].b = b;

			created++;
			if (created >= spawn_count) break;
		}
	}
}

//エフェクト描画
void particle_render()
{
	for (int i = 0; i < PARTICLE_NUMBER; i++)
	{
		if (!particle[i].active) continue;

		particle[i].life--;
		if (particle[i].life <= 0)
		{
			particle[i].active = false;
			continue;
		}

		particle[i].x += particle[i].vx;
		particle[i].y += particle[i].vy;

		particle[i].vx *= 0.92f;
		particle[i].vy *= 0.92f;

		particle[i].alpha = (float)particle[i].life / (float)particle[i].max_life;

		primitive::rect(
			particle[i].x - particle[i].size * 0.5f,
			particle[i].y - particle[i].size * 0.5f,
			particle[i].size,
			particle[i].size,
			0,
			particle[i].r,
			particle[i].g,
			particle[i].b,
			particle[i].alpha,
			1
		);
	}
}