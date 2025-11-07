#pragma once
#include "../chessman/chessman.h"

class bishop : public chessman {
public:
    bishop() = delete;

    bishop(boardcoordinate initial_pos, color c);

protected:
    void fill_available_relative_position_list(board_grid board_state) override;
};
