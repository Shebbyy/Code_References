#include "knight.h"

knight::knight(const boardcoordinate initial_pos, const color c): chessman(initial_pos, c, 'n', false)
{
}

void knight::fill_available_relative_position_list(board_grid board_state) {
    fill_available_position_list_knight_mirror(board_state, {2, 1});
    fill_available_position_list_knight_mirror(board_state, {-2, 1});
    fill_available_position_list_knight_mirror(board_state, {2, -1});
    fill_available_position_list_knight_mirror(board_state, {-2, -1});
}

void knight::fill_available_position_list_knight_mirror(board_grid board_state, boardcoordinate rel_pos) {
    boardcoordinate dest_pos = current_position + rel_pos;

    if (destination_valid_or_enemy(board_state, dest_pos)) {
        relative_available_positions.push_front(rel_pos);
    }

    std::swap(rel_pos.x, rel_pos.y);
    dest_pos = current_position + rel_pos;

    if (destination_valid_or_enemy(board_state, dest_pos)) {
        relative_available_positions.push_front(rel_pos);
    }
}
