#include "chessboard.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>

#include "../figures/bishop/bishop.h"
#include "../figures/king/king.h"
#include "../figures/knight/knight.h"
#include "../figures/pawn/pawn.h"
#include "../figures/queen/queen.h"
#include "../figures/rook/rook.h"

constexpr int ASCII_LOWERCHAR_START = 97;

chessboard::chessboard() {
    init_game_board();
}

void chessboard::init_game_board() {
    for (int i = 0; i < COL_WIDTH; i++) {
        king * upKing = nullptr;
        king * lowKing = nullptr;

        switch (i) {
        case 0:
        case 7:
            board[0][i] = new rook({i, 0}, UPPERCASE);
            board[7][i] = new rook({i, 7}, LOWERCASE);
            break;
        case 1:
        case 6:
            board[0][i] = new knight({i, 0}, UPPERCASE);
            board[7][i] = new knight({i, 7}, LOWERCASE);
            break;
        case 2:
        case 5:
            board[0][i] = new bishop({i, 0}, UPPERCASE);
            board[7][i] = new bishop({i, 7}, LOWERCASE);
            break;
        case 3:
            board[0][i] = new queen({i, 0}, UPPERCASE);
            board[7][i] = new queen({i, 7}, LOWERCASE);
            break;
        case 4:
            upKing = new king({i, 0}, UPPERCASE);
            board[0][i] = upKing;
            king_map.insert({UPPERCASE, upKing});

            lowKing = new king({i, 7}, LOWERCASE);
            board[7][i] = lowKing;
            king_map.insert({LOWERCASE, lowKing});
            break;
        default:
            // Dürfte nie auftreten, da sonst zu viele Spalten vorhanden wären!
            assert(false);
            break;
        }

        board[1][i] = new pawn({i, 1}, UPPERCASE);
        board[6][i] = new pawn({i, 6}, LOWERCASE);
    }
}

void chessboard::print_board() {
    std::cout << std::format("  | a  b  c  d  e  f  g  h |  \n");
    std::cout << std::format("--+------------------------+--\n");
    for (int i = ROW_HEIGHT - 1; i >= 0; i--) {
        std::cout << std::format("{} |", ROW_HEIGHT - i);

        for (int j = 0; j < COL_WIDTH; j++) {
            std::string out = (i + j) % 2 ? "*" : ".";

            if (board[i][j]) {
                out = board[i][j]->get_symbol();
            }

            cell_wrapper wrappers = get_cell_wrappers(boardcoordinate({j, i}));

            std::cout << wrappers.prefix << out << wrappers.postfix;
        }

        std::cout << std::format("| {}\n", ROW_HEIGHT - i);
    }
    std::cout << std::format("--+------------------------+--\n");
    std::cout << std::format("  | a  b  c  d  e  f  g  h |  \n");
}

void chessboard::select_pos(boardcoordinate selection_pos) {
    selected_pos = new boardcoordinate();
    selected_pos->x = selection_pos.x;
    selected_pos->y = selection_pos.y;
}

chessman* chessboard::get_position_figure(boardcoordinate dest_coord) const {
    return board[dest_coord.y][dest_coord.x];
}

bool chessboard::move_to_pos(boardcoordinate destination_pos) {
    // Selektion Rückgängig gemacht, keine Bewegung durchzuführen
    if (!selected_pos)
        return true;

    bool initial_check_cond = check_check_condition();

    chessman * figure = get_selected_figure();
    if (figure->can_move_to_position(destination_pos, board)) {
        boardcoordinate oldPos = figure->get_position();
        chessman * destFigure = get_position_figure(destination_pos);

        board[destination_pos.y][destination_pos.x] = board[oldPos.y][oldPos.x];
        board[oldPos.y][oldPos.x] = nullptr;

        // Kein Self-Check, dh. ok!
        // !initial_check_cond, da keine Check-Mate Ermittlung implementiert wurde
        if (   !check_check_condition()
            || initial_check_cond) {
            figure->update_position(destination_pos);

            if (destFigure) {
                std::cout << std::format("Die Figur {} wurde geschlagen!\n", destFigure->get_symbol());

                if (destFigure->is_essential()) {
                    king_map.at(destFigure->get_color()) = nullptr;
                }
                delete destFigure;
            }

            return true;
        }

        board[oldPos.y][oldPos.x] = board[destination_pos.y][destination_pos.x];
        board[destination_pos.y][destination_pos.x] = destFigure;

        std::cout << "Warnung: Spieler hätte sich selbst in ein Check-State gebracht!\n";
    }
    return false;
}

boardcoordinate chessboard::request_player_input(input_type type, const std::string& selection_format_string, const std::string& error_append) {
    std::string player_name = current_turn == UPPERCASE ? "Grossbuchstaben" : "Kleinbuchstaben";

    bool validInput = false;
    std::string input;
    boardcoordinate coord = {0, 0};
    while (!validInput) {
        std::cout << std::vformat(selection_format_string, std::make_format_args(player_name));


        std::cin >> input;
        coord = {static_cast<int>(input.at(0)) - ASCII_LOWERCHAR_START, ROW_HEIGHT - atoi(&input.at(1))};

        // 2. Validate-Funktion musste leider so gelöst werden, da function-pointer von Member-Functions große Probleme verursacht hat
        validInput =    input.length() == 2
                     && type == SELECT ? validate_player_selection(coord)
                                       : validate_figure_destination_valid(coord);

        if (!validInput) {
            std::cout << "Die eingegebene Position ist ungültig!\nDie Position muss im Format ColRow (zB. a2) sein " << error_append << "\n";
        }
    }

    return coord;
}

boardcoordinate chessboard::request_player_selection_input() {
    return request_player_input(SELECT, "Spieler {}: Wähle eine deiner Figuren aus (Format Bsp. a2)!\n", "und eine Figur des Spielers enthalten");
}

boardcoordinate chessboard::request_player_movement_input() {
    boardcoordinate destination = request_player_input(DESTINATION, "Spieler {}: Wähle die Zielposition der Figur aus! (mit [] markiert)\nVerwende die selektierte Position um die Figur zurückzustellen\n", "und muss zu den markierten Feldern gehören!");

    // Falls SelectionPos verwendet wird --> Figur zurückstellen und erneute Selektion ermöglichen
    if (destination == *selected_pos) {
        unselect();
    }

    return destination;
}

bool chessboard::check_win_condition() {
    for (auto const &king : king_map) {
        // Funktioniert, da nur max. 2 Elemente in der Map liegen.
        if (   king.first != current_turn
            && !king.second) {
            unselect(); // Damit beim Draw am Ende kein Feld selektiert ist.
            return true;
        }
    }
    return false;
}

bool chessboard::check_check_condition() {
    // Hier kann über Board iteriert werden, da die Actions von der gerade bewegten Figur irrelevant sind, da keine Gegnerfigur.
    // Deshalb kann update_position von der bewegten Figur erst nach dem Check hier durchgeführt werden
    for (auto &row : board) {
        for (auto &cell : row) {
            if (   !cell
                || cell->get_color() == current_turn) {
                continue;
            }

            for (auto const & [x, y] : cell->get_possible_positions(board)) {
                // Gegnerische Figur kann direkt auf King fahren, dieser steht also im Check
                // Nachdem auf aktuellen Turn gesehen --> Spieler hat eigene Figur verschoben sodass er sich selbst ins Check gebracht hat
                // nicht erlaubt.
                if (   board[y][x]
                    && board[y][x]->is_essential()
                    && board[y][x]->get_color() == current_turn) {
                    return true;
                }
            }
        }
    }

    return false;
}

void chessboard::switch_turns() {
    // Kein Turn Wechsel, wenn Selection rückgängig gemacht wurde
    if (!selected_pos)
        return;

    current_turn = current_turn == UPPERCASE ? LOWERCASE : UPPERCASE;

    unselect();
}

void chessboard::unselect() {
    delete selected_pos;
    selected_pos = nullptr;
}

chessman* chessboard::get_selected_figure() const {
    return board[selected_pos->y][selected_pos->x];
}

cell_wrapper chessboard::get_cell_wrappers(boardcoordinate coord) {
    if (selected_pos) {
        if (coord == *selected_pos) {
            return {'(', ')'};
        }

        std::list<boardcoordinate> possible_pos = get_selected_figure()->get_possible_positions(board);

        auto found_iter = std::find(possible_pos.begin(), possible_pos.end(), coord);
        if (found_iter != possible_pos.end()) {
            return {'[', ']'};
        }
    }

    return {' ', ' '};
}

chessboard::~chessboard() {
    for (auto &row : board) {
        // cell enthält Chessman Pointer, drin liegen aber andere Figure-Elemente, dh. muss statt delete[] jede Zelle einzeln gecleart werden.
        for (auto &cell : row)
        {
            delete cell;
        }
    }

    delete selected_pos;
}

std::string chessboard::get_winner() const {
    if (king_map.at(UPPERCASE) == nullptr) {
        return "LOWERCASE";
    }

    if (king_map.at(LOWERCASE) == nullptr) {
        return "UPPERCASE";
    }

    return "";
}

bool chessboard::validate_player_selection(boardcoordinate coord) const {
    return coord.x >= 0
        && coord.x <= 7
        && coord.y >= 0
        && coord.y <= 7
        && board[coord.y][coord.x]
        && board[coord.y][coord.x]->get_color() == current_turn;
}

bool chessboard::validate_figure_destination_valid(boardcoordinate coord) {
    if (*selected_pos == coord) {
        return true;
    }

    chessman * figure = get_selected_figure();

    for (auto &dest_coord : figure->get_possible_positions(board)) {
        if (coord == dest_coord) {
            return true;
        }
    }

    return false;
}