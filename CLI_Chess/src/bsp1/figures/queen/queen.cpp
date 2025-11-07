//
// Created by Shebby on 12/01/2025.
//

#include "queen.h"

queen::queen(const boardcoordinate initial_pos, const color c): chessman(initial_pos, c, 'q', false)
{
}

void queen::fill_available_relative_position_list(board_grid board_state) {
    for (int row = -1; row <= 1; row++) {
        for (int col = -1; col <= 1; col++) {
            boardcoordinate rel_pos = {current_position.x + col, current_position.y + row};
            if (!rel_pos.is_in_board()) {
                continue;
            }

            if (   !board_state[rel_pos.y][rel_pos.x]
                || board_state[rel_pos.y][rel_pos.x]->get_color() != get_color()) {
                fill_relative_position_list_up_to_blocker(board_state, {col, row});
            }
        }
    }
}
