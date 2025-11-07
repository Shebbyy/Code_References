#pragma once
#include "../chessman/chessman.h"

class queen : public chessman {
public:
    queen() = delete;

    queen(boardcoordinate initial_pos, color c);

protected:
    void fill_available_relative_position_list(board_grid board_state) override;
};
