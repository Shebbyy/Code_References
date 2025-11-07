//
// Created by Shebby on 12/01/2025.
//

#include "pawn.h"

pawn::pawn(const boardcoordinate initial_pos, const color c): chessman(initial_pos, c, 'p', false) {}

void pawn::update_position(boardcoordinate new_pos)
{
    chessman::update_position(new_pos);
    is_first_move = false;
}

void pawn::fill_available_relative_position_list(board_grid board_state) {
    const int y = piece_color == UPPERCASE ? 1 : -1;

    boardcoordinate dest_pos = get_position() + boardcoordinate({0, y});
    chessman * dest_figure = board_state[dest_pos.y][dest_pos.x];
    if (!dest_figure)
    {
        relative_available_positions.push_front({0, y});

        dest_pos = get_position() + boardcoordinate({0, y});
        dest_figure = board_state[dest_pos.y][dest_pos.x];
        if (   is_first_move
            && !dest_figure) {
            relative_available_positions.push_front({0, y * 2});
        }
    }

    add_attack_field(-1, y, dest_pos, board_state);
    add_attack_field(1, y, dest_pos, board_state);
}

void pawn::add_attack_field(int x_delta, int y_delta, const boardcoordinate &pos, board_grid board_state) {
    chessman * dest_figure = board_state[pos.y][pos.x + x_delta];
    if (   dest_figure
        && dest_figure->get_color() != get_color())
    {
        relative_available_positions.push_front({x_delta, y_delta});
    }
}
