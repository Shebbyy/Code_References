#include "bishop.h"

bishop::bishop(const boardcoordinate initial_pos, const color c): chessman(initial_pos, c, 'b', false)
{}

void bishop::fill_available_relative_position_list(board_grid board_state) {
    fill_relative_position_list_up_to_blocker(board_state, {1, 1});
    fill_relative_position_list_up_to_blocker(board_state, {-1, 1});
    fill_relative_position_list_up_to_blocker(board_state, {1, -1});
    fill_relative_position_list_up_to_blocker(board_state, {-1, -1});
}