#pragma once
#include "all.h"

#define HP_MAX (3)
#define BULLET_SPEED (200)
#define EFFICACY_MAX (6)

// ‰æ‘œ
#define POWER_EFFICACY_SCALE     (1.0f)
#define POWER_EFFICACY_TEX_W     (384.0f)
#define POWER_EFFICACY_TEX_H     (768.0f)
#define POWER_EFFICACY_PIVOT_X   (192.0f)
#define POWER_EFFICACY_PIVOT_Y   (384.0f)

extern OBJ2D power_efficacy[EFFICACY_MAX];

void power_efficacy_init();
void power_efficacy_update();
void power_efficacy_render();
void power_efficacy_deinit();
void Choice_num(int choice_num);

extern void HP(int choice_num);
extern void Bullet_speed(int choice_num);
extern void Bullet_quantity(int choice_num);
extern void Bullet_diffusion(int choice_num);
extern void Bullet_scale(int choice_num);
extern void Bullet_rad(int choice_num);

extern void loser(int choice_num);

