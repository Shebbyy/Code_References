#pragma once
#include "../chessman/chessman.h"

class knight : public chessman {
public:
    knight() = delete;

    knight(boardcoordinate initial_pos, color c);

protected:
    void fill_available_relative_position_list(board_grid board_state) override;

    void fill_available_position_list_knight_mirror(board_grid board_state, boardcoordinate rel_pos);
};
