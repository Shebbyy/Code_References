#pragma once

#include "./tetrimino.h"

void initialize_renderer (int rows, int cols, on_key_type * on_key);
bool renderer_is_open    (void);
void shut_down_renderer  (void);
void render_block (position_type position, color_type color);
void begin_frame (void);
void end_frame   (void);
