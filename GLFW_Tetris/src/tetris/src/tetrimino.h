#pragma once

#pragma region Macros

#define COLOR_COUNT 8
// is default if you want to add new figures with more than 4 blocks change this value
// and add new constructor function for the figure and add to enum figure_t for random
#define BLOCK_COUNT 4

#pragma endregion

#include "./primitives.h"

#pragma region Types

typedef enum {
   color_black   = 0,
   color_red     = 255U <<  0U,
   color_green   = 255U <<  8U,
   color_blue    = 255U << 16U,
   color_yellow  = color_red   | color_green,
   color_magenta = color_red   | color_blue,
   color_cyan    = color_green | color_blue,
   color_white   = color_red   | color_green | color_blue,
} color_type;

typedef enum
{
	clockwise = 0,
	counterclockwise = 1
} figure_direction;

typedef enum {
	I,
	O,
	L,
	J,
	T,
	S,
	Z
	// add new figures here
} figure_t;

static const figure_t figures[] = { I, O, L, J, T, S, Z };

typedef struct {
   color_type    color;
   position_type position;
} block_type;

typedef struct tetromino {
	block_type blocks[BLOCK_COUNT];
	int center;
	int bricks_count;
} tetromino;

#pragma endregion

#pragma region constructor

tetromino* new_tetromino(figure_t type);
tetromino* new_rand_tetromino(void);

#pragma endregion

#pragma region default color settings

GLfloat normalize_blue  (color_type color);
GLfloat normalize_green (color_type color);
GLfloat normalize_red   (color_type color);
char const* string_from_color(color_type const color);

#pragma endregion

#pragma region position functions

bool set_horizontal(tetromino** brick, int const dx);
bool set_vertical(tetromino** brick, int const dy);
bool rotate_block(tetromino** brick, figure_direction direction);

#pragma endregion

