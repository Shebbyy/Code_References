#include <iostream>

#if __has_include(<vld.h>)
#include <vld.h>
#endif

#include "board/chessboard.h"

int main()
{
    chessboard * board = new chessboard();

    bool winner = false;

    while (!winner) {
        board->switch_turns();
        board->print_board();
        boardcoordinate coord = board->request_player_selection_input();
        board->select_pos(coord);

        bool valid_movement = false;

        while (!valid_movement) {
            board->print_board();
            boardcoordinate destination_coord = board->request_player_movement_input();

            valid_movement = board->move_to_pos(destination_coord);

            if (!valid_movement) {
                std::cout << "Die Figur kann sich nicht auf dieses Feld bewegen! Gib ein anderes an.\n";
            }
        }

        winner = board->check_win_condition();
    }

    board->print_board();

    std::string winner_color = board->get_winner();

    std::cout << std::format("Spieler {} hat gewonnen!\n", winner_color);

    delete board;
}
