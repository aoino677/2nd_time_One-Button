#pragma once
#include "all.h"

#define EFFICACY_MAX (3)

void power_up_init();
void power_up_update();
void power_up_render();
void power_up_deinit();

extern void Rapidfire();
extern void power();
extern void HP();