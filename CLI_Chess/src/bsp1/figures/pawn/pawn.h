#pragma once
#include "../chessman/chessman.h"

class pawn : public chessman {
public:
    pawn() = delete;

    pawn(boardcoordinate initial_pos, color c);

    void update_position(boardcoordinate new_pos) override;

protected:
    bool is_first_move = true;
    void fill_available_relative_position_list(board_grid board_state) override;
    void add_attack_field(int x_delta, int y_delta, const boardcoordinate &actual_pos, board_grid board_state);
};
