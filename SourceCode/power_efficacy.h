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

extern void HP();
extern void Bullet_speed();
extern void Bullet_quantity();
extern void Bullet_diffusion();
extern void Bullet_scale();
extern void Bullet_rad();

extern void loser();

