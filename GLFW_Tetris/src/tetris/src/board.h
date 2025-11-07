#pragma once

#include "./primitives.h"
#include "./tetrimino.h"

#pragma region Macros

#define N_COLS 11
#define N_ROWS 22

#pragma endregion

extern int count_for_speed;

bool is_valid_position (position_type position);
bool set_block_to_board (tetromino block);
bool render_tetromino_block (tetromino const* block);
void dispose_board(void);

