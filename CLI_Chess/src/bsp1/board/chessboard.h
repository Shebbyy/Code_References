#pragma once

#include <map>
#include <string>

#include "../figures/chessman/chessman.h"
#include "../figures/king/king.h"

struct cell_wrapper {
    char prefix;
    char postfix;
};

class chessboard {
public:

    chessboard();

    void init_game_board();
    void print_board();

    void select_pos(boardcoordinate selection_pos);

    chessman* get_position_figure(boardcoordinate boardcoordinate) const;

    bool move_to_pos(boardcoordinate destination_pos);

    [[nodiscard]] boardcoordinate request_player_selection_input();

    boardcoordinate request_player_movement_input();

    bool check_win_condition();

    bool check_check_condition();

    void switch_turns();

    ~chessboard();

    [[nodiscard]] std::string get_winner() const;

private:
    enum input_type {
        SELECT,
        DESTINATION
    };

    board_grid board = {nullptr};
    boardcoordinate * selected_pos = nullptr;
    color current_turn = UPPERCASE;
    std::map<color, king *> king_map;

    [[nodiscard]] bool validate_player_selection(boardcoordinate coord) const;
    [[nodiscard]] bool validate_figure_destination_valid(boardcoordinate coord);

    void unselect();

    boardcoordinate request_player_input(input_type, const std::string& string, const std::string& error_str);

    [[nodiscard]] chessman * get_selected_figure() const;

    cell_wrapper get_cell_wrappers(boardcoordinate coord);

};
