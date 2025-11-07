#include "chessman.h"

#include <cassert>
#include <locale>

color chessman::get_color() const {
    return piece_color;
}

char chessman::get_symbol() const {
    if (piece_color == UPPERCASE) {
        return static_cast<char>(std::toupper(piece_symbol));
    }
    return piece_symbol;
}

boardcoordinate chessman::get_position() const {
    return current_position;
}

bool chessman::is_essential() const {
    return essential;
}

void chessman::update_position(boardcoordinate new_pos) {
    current_position = new_pos;
}

bool chessman::can_move_to_position(boardcoordinate dest_pos, board_grid board_state) {
    relative_available_positions.clear();
    this->fill_available_relative_position_list(board_state);

    assert(board_state);

    for (auto &listNode : relative_available_positions) {
        if (   current_position.x + listNode.x == dest_pos.x
            && current_position.y + listNode.y == dest_pos.y)
        {
            return true;
        }
    }

    return false;
}

std::list<boardcoordinate> chessman::get_possible_positions(board_grid board_state) {
    relative_available_positions.clear();
    fill_available_relative_position_list(board_state);

    std::list<boardcoordinate> possible_dest_positions;
    for (const auto & [x, y] : relative_available_positions) {
        possible_dest_positions.push_front({
            x + current_position.x,
            y + current_position.y
        });
    }

    return possible_dest_positions;
}

chessman::chessman(boardcoordinate initial_pos, const color c, const char s, const bool e) {
    piece_symbol = s;
    piece_color = c;
    essential = e;
    relative_available_positions = std::list<boardcoordinate>();
    current_position = initial_pos;
}

void chessman::fill_relative_position_list_up_to_blocker(board_grid board_state, boardcoordinate step_delta) {
    boardcoordinate iter_pos = current_position;

    iter_pos += step_delta;
    while (   iter_pos.is_in_board()
           && !board_state[iter_pos.y][iter_pos.x]) {
        relative_available_positions.push_front({iter_pos.x - current_position.x, iter_pos.y - current_position.y});
        iter_pos += step_delta;
           }

    // Falls Gegner-Figur das blockierende Element ist, Catch ermöglichen, eg. Feld für Movement hinzufügen
    if (   iter_pos.is_in_board()
        && board_state[iter_pos.y][iter_pos.x]
        && board_state[iter_pos.y][iter_pos.x]->get_color() != get_color())
    {
        relative_available_positions.push_front({iter_pos.x - current_position.x, iter_pos.y - current_position.y});
    }
}

bool chessman::destination_valid_or_enemy(chessman* board_state[8][8], boardcoordinate dest_pos) const {
    return dest_pos.is_in_board()
        && (!board_state[dest_pos.y][dest_pos.x]
           || board_state[dest_pos.y][dest_pos.x]->get_color() != get_color());
}
