#pragma once

#include <list>

#include "../../board/coordinate/coordinate.h"

class chessman;
typedef chessman * board_grid[ROW_HEIGHT][COL_WIDTH];

enum color
{
    UPPERCASE,
    LOWERCASE,
};

class chessman {
public:
    chessman() = delete;

    [[nodiscard]] color get_color() const;
    [[nodiscard]] char  get_symbol() const;
    [[nodiscard]] boardcoordinate  get_position() const;
    [[nodiscard]] bool  is_essential() const;
    void virtual update_position(boardcoordinate new_pos);

    bool can_move_to_position(boardcoordinate dest_pos, board_grid board_state);

    virtual ~chessman() = default;
    [[nodiscard]] std::list<boardcoordinate> get_possible_positions(board_grid board_state);

protected:
    boardcoordinate current_position = {-1, -1};
    color piece_color = UPPERCASE;
    char  piece_symbol {};
    bool  essential = false;
    std::list<boardcoordinate> relative_available_positions = {};

    chessman(boardcoordinate initial_pos, color c, char s, bool e);

    void virtual fill_available_relative_position_list(board_grid board_state) = 0;

    void fill_relative_position_list_up_to_blocker(board_grid board_state, boardcoordinate step_delta);

    bool destination_valid_or_enemy(board_grid board_state, boardcoordinate dest) const;
};

