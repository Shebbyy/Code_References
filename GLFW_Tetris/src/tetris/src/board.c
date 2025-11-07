#include "./board.h"
#include "./window.h"
#include "./tetrimino.h"

static block_type* g_board[N_COLS * N_ROWS];
int count_for_speed = 1;

#pragma region check collisions

bool is_collision_with_blocks(position_type const position)
{
	// check if the position is already occupied
	for (int i = 0; g_board[i]; i++)
		if (g_board[i]->position.x == position.x && g_board[i]->position.y == position.y)
			return  true;

	return false;
}

bool is_valid_position (position_type const position) {
	// check if the position is already occupied
	if (g_board[0] && is_collision_with_blocks(position))
		return false;

	// check if the position is inside the board
	return position.x < N_COLS && position.x >= 0
		&& position.y < N_ROWS && position.y >= 0;
}

bool is_board_full(void)
{
	// run through all rows and check if there is a brick
	for (int i = 0; i < N_ROWS; i++)
	{
		bool is_brick = false;
		// check if there is a brick in the row
		for (int j = 0; g_board[j] && !is_brick; j++)
			if (g_board[j]->position.y == i)
				is_brick = true;

		// if there is no brick in the row, the board is not full
		if (!is_brick)
			return true;
	}
		
	return false;
}

#pragma endregion

#pragma region set bricks to board

void repair_board(void)
{
	const int len = N_COLS * N_ROWS;
	block_type** new_board[N_COLS * N_ROWS] = { NULL };
	int index = 0;
	int last_index = 0;

	// add bricks to new board to eliminate gaps
	for (int i = 0; i < len; ++i)
		if (g_board[i])
			new_board[index++] = &g_board[i], last_index = i;
		
			
	// add NULL pointers to the end of the board
	for (int i = 0; i <= last_index && i < len; ++i)
		g_board[i] = new_board[i] ? *new_board[i] : NULL;
}

void remove_bricks(int const y_pos)
{
	count_for_speed++;
	// remove all bricks in the row
	for (int i = 0; i < N_COLS * N_ROWS && g_board[i] != NULL; i++)
		if (g_board[i]->position.y == y_pos)
			free(g_board[i]),
			g_board[i] = NULL;

	// sort the board -> NULL to end
	repair_board();
}

void update_bricks_after_remove(int const y_pos)
{
	// remove all bricks in the row with board sorting
	remove_bricks(y_pos);

	// move all bricks above the removed row one row down
	for (int i = 0; i < N_COLS * N_ROWS && g_board[i] != NULL; i++)
		if (g_board[i]->position.y > y_pos)
			g_board[i]->position.y -= 1;
}

void line_full_of_blocks(int const row)
{
	// check if the row is full of bricks
	for (int i = 0; i < N_ROWS; i++) {
		int count = 0;
		// count the bricks in the row
		for (int j = 0; j < N_ROWS*N_ROWS && g_board[j] != NULL; j++)
			if (g_board[j]->position.y == i)
				count++;

		// if the row is full of bricks, remove them
		if (count == N_COLS)
			// new row should be checked again
			update_bricks_after_remove(i--);
	}
}

bool set_block_to_board(tetromino block)
{
	int index = 0;

	// find the first free space in the board
	while (g_board[index]) index++;

	for (int i = 0; i < block.bricks_count; i++)
	{
		int const ind = index + i;
		g_board[ind] = (block_type*)malloc(sizeof(block_type));
		// check if the memory allocation was successful
		if (!g_board[ind])
			return false;
		// copy the block to the board
		memcpy(g_board[ind], &block.blocks[i], sizeof(block_type));
	}

	// check if a row is full of bricks
	line_full_of_blocks(block.blocks[0].position.y);

	return true;
}

#pragma endregion

#pragma region render blocks and clean up

void dispose_board(void)
{
	repair_board();
	// free all bricks
	for (int i = 0; i < N_COLS * N_ROWS && g_board[i]; i++)
		if (g_board[i])
			free(g_board[i]), g_board[i] = NULL;
}

bool render_tetromino_block(tetromino const* block)
{
	// check if the board is full
	if (!is_board_full())
		return false;

	// render the tetromino block
	if (block)
		for (int i = 0; i < block->bricks_count; i++)
			render_block(block->blocks[i].position, block->blocks[i].color);

	// render the board
	for (int i = 0; g_board[i]; i++)
		render_block(g_board[i]->position, g_board[i]->color);

	return true;
}

#pragma endregion
