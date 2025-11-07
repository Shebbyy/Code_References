#include "./window.h"
#include "./board.h"
#include "./primitives.h"

#include <time.h>

#pragma region Macros

# define DROP_OFF_HORIZONTAL (N_COLS / 2)
# define DROP_OFF_VERTICAL (N_ROWS - 1)
# define DEFAULT_BRICKS_COUNT 4

# define MIDDLE_BLOCK_I 1
# define MIDDLE_BLOCK_O -1
# define MIDDLE_BLOCK_L 1
# define MIDDLE_BLOCK_J 1
# define MIDDLE_BLOCK_T 1
# define MIDDLE_BLOCK_S 0
# define MIDDLE_BLOCK_Z 1
// define middle block for new figures here

#pragma endregion 

#pragma region Normalize color functions

color_type get_random_color(void)
{
	// initialize the random number generator
	srand((unsigned int)time(NULL));

	// without black
	color_type colors[COLOR_COUNT - 1] = {
	// color_black,
	color_red,
	color_green,
	color_blue,
	color_yellow,
	color_magenta,
	color_cyan,
	color_white
	};

	// return a random color
	return colors[rand() % (sizeof(colors) / sizeof(colors[0]))];
}

GLfloat normalize_blue (color_type const color) {
   return (GLfloat) (color & color_blue) / color_blue;
}

GLfloat normalize_green (color_type const color) {
   return (GLfloat) (color & color_green) / color_green;
}

GLfloat normalize_red (color_type const color) {
   return (GLfloat) (color & color_red) / color_red;
}

char const* string_from_color(color_type const color)
{
	switch (color)
	{
	case color_black: return "black";
	case color_red: return "red";
	case color_green: return "green";
	case color_blue: return "blue";
	case color_yellow: return "yellow";
	case color_magenta: return "magenta";
	case color_cyan: return "cyan";
	case color_white: return "white";
	default: return "unknown";
	}
}

#pragma endregion

#pragma region Tetrominobuilder

tetromino* created_tetromino(position_type (*fill_block)(int const brick_index, int const bricks), int const center_block, int const bricks_count)
{
	tetromino* t = (tetromino*)malloc(sizeof(tetromino));
	
	if (!t)
		return NULL;

	// create the blocks
	for (int i = 0; i < bricks_count; i++)
	{
		t->blocks[i].color = get_random_color();
		// set the position of every block
		t->blocks[i].position = fill_block(i, bricks_count);
	}

	// set the center block
	t->center = center_block;
	t->bricks_count = bricks_count;

	return t;
}

// add new figures here

position_type fill_block_i(int const brick_index, int const bricks)
{
	return make_position(DROP_OFF_HORIZONTAL, DROP_OFF_VERTICAL - brick_index);
}

position_type fill_block_o(int const brick_index, int const bricks)
{
	return make_position(DROP_OFF_HORIZONTAL + brick_index % 2, DROP_OFF_VERTICAL - brick_index / 2);
}

position_type fill_block_l(int const brick_index, int const bricks)
{
	return make_position(DROP_OFF_HORIZONTAL + brick_index % 3 - 1, DROP_OFF_VERTICAL - brick_index / 3);
}

position_type fill_block_j(int const brick_index, int const bricks) {
	return make_position(DROP_OFF_HORIZONTAL - brick_index % 3 + 1, DROP_OFF_VERTICAL - brick_index / 3);
}

position_type fill_block_t(int const brick_index, int const bricks) {
	int const x_pos = brick_index < bricks - 1 ? DROP_OFF_HORIZONTAL + brick_index % 3 - 1 : DROP_OFF_HORIZONTAL;
	return make_position(x_pos, DROP_OFF_VERTICAL - brick_index / 3);
}

position_type fill_block_s(int const brick_index, int const bricks) {
	int const x_pos = brick_index < bricks - 1 ? DROP_OFF_HORIZONTAL + brick_index % 2 : DROP_OFF_HORIZONTAL - 1;
	return make_position(x_pos, DROP_OFF_VERTICAL - brick_index / 2);
}

position_type fill_block_z(int const brick_index, int const bricks) {
	int const x_pos = brick_index < bricks - 2 ? DROP_OFF_HORIZONTAL + brick_index % 2 : DROP_OFF_HORIZONTAL + 1 + brick_index % 2;
	return  make_position(x_pos, DROP_OFF_VERTICAL - brick_index / 2);
}

# pragma endregion

#pragma region Tetromino functions

tetromino* new_tetromino(figure_t type)
{
	switch (type)
	{
	case I:
		return created_tetromino(&fill_block_i, MIDDLE_BLOCK_I, DEFAULT_BRICKS_COUNT);
	case O:
		return created_tetromino(&fill_block_o, MIDDLE_BLOCK_O, DEFAULT_BRICKS_COUNT);
	case L:
		return created_tetromino(&fill_block_l, MIDDLE_BLOCK_L, DEFAULT_BRICKS_COUNT);
	case J:
		return created_tetromino(&fill_block_j, MIDDLE_BLOCK_J, DEFAULT_BRICKS_COUNT);
	case T:
		return created_tetromino(&fill_block_t, MIDDLE_BLOCK_T, DEFAULT_BRICKS_COUNT);
	case S:
		return created_tetromino(&fill_block_s, MIDDLE_BLOCK_S, DEFAULT_BRICKS_COUNT);
	case Z:
		return created_tetromino(&fill_block_z, MIDDLE_BLOCK_Z, DEFAULT_BRICKS_COUNT);
		// add new figures here
	}

	return NULL;
}

tetromino* new_rand_tetromino(void)
{
	srand((unsigned int)time(NULL));
	// get random figure
	figure_t const new_type = figures[rand() % (sizeof(figures) / sizeof(figures[0]))];
	return new_tetromino(new_type);
}

#pragma endregion

#pragma region Set position functions

bool set_vert_hor_position(tetromino** brick, int const dx, int const dy)
{
	bool result = true;
	tetromino* new_brick = (tetromino*)malloc(sizeof(tetromino));
	if (!new_brick)
		return false;
	// copy the old brick
	memcpy(new_brick, *brick, sizeof(tetromino));

	// set the new position to old brick
	for (int i = 0; i < (*brick)->bricks_count; i++)
		result &= set_position(&new_brick->blocks[i].position, dx, dy);

	// if the new position is valid, copy the new brick to the old brick
	if (result)
	{
		free(*brick);
		*brick = new_brick;
		return result;
	}

	// free the new brick
	free(new_brick);
	new_brick = NULL;
	return result;
}

bool set_horizontal(tetromino** brick, int const dx)
{
	return set_vert_hor_position(brick, dx, 0);
}

bool set_vertical(tetromino** brick, int const dy)
{
	return set_vert_hor_position(brick, 0, dy);
}

bool rotate_block(tetromino** brick, figure_direction direction)
{
	// check if the brick is valid
	if (!brick || (*brick)->center < 0 || (*brick)->center >= (*brick)->bricks_count)
		return false;

	// create a new brick
	tetromino* new_brick = (tetromino*)malloc(sizeof(tetromino));
	if (!new_brick)
		return false;

	// get the center block index
	int const r_index = (*brick)->center;
	memcpy(new_brick, *brick, sizeof(tetromino));

	bool is_valid = true;

	for (int i = 0; i < (*brick)->bricks_count; i++)
	{
		// skip the center block
		if (i == r_index)
			continue;
		
		// calculate the new position
		int x_radius = new_brick->blocks[i].position.x - new_brick->blocks[r_index].position.x;
		int y_radius = new_brick->blocks[i].position.y - new_brick->blocks[r_index].position.y;

		// check the direction
		x_radius = direction != clockwise ? -x_radius : x_radius;
		y_radius = direction != clockwise ? y_radius : -y_radius;

		// set the new position
		new_brick->blocks[i].position = make_position(new_brick->blocks[r_index].position.x + y_radius, new_brick->blocks[r_index].position.y + x_radius);
		// check if the new position of current brick is valid
		is_valid &= is_valid_position(new_brick->blocks[i].position);
	}

	// if the new brick is valid, copy the new brick to the old brick
	if (is_valid)
		memcpy(*brick, new_brick, sizeof(tetromino));

	// free the new brick
	free(new_brick);
	return is_valid;

}

#pragma endregion
