#pragma once
#include "../chessman/chessman.h"

class rook : public chessman {
public:
    rook() = delete;

    rook(boardcoordinate initial_pos, color c);

protected:
    void fill_available_relative_position_list(board_grid board_state) override;
};
