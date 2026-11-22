#pragma once

#define TUTORIAL_BUTTON_TEX_SIZE_W (1000)
#define TUTORIAL_BUTTON_TEX_SIZE_H  (200)
#define TUTORIAL_BUTTON_PIVOT_X (TUTORIAL_BUTTON_TEX_SIZE_W / 2)
#define TUTORIAL_BUTTON_PIVOT_Y (TUTORIAL_BUTTON_TEX_SIZE_H / 2)
#define TUTORIAL_BUTTON_SCALE_X (0.5f)
#define TUTORIAL_BUTTON_SCALE_Y (0.5f)

void tutorial_button_init();
void tutorial_button_deinit();
void tutorial_button_update();
void tutorial_button_render();
