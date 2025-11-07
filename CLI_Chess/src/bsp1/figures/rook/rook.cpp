//
// Created by Shebby on 12/01/2025.
//

#include "rook.h"

rook::rook(const boardcoordinate initial_pos, const color c): chessman(initial_pos, c, 'r', false)
{
}

void rook::fill_available_relative_position_list(board_grid board_state) {
    fill_relative_position_list_up_to_blocker(board_state, {1, 0});
    fill_relative_position_list_up_to_blocker(board_state, {-1, 0});
    fill_relative_position_list_up_to_blocker(board_state, {0, 1});
    fill_relative_position_list_up_to_blocker(board_state, {0, -1});
}
