#pragma once

#include <map>
#include "../config/config.h"
#include "../game_window/game_window.h"
#include "../brick/brick.h"
#include "../ball/ball.h"
#include "../paddle/paddle.h"

class breakout_game {
public:
    breakout_game();
    static coordinate get_init_ball_coord(coordinate paddle_coord);


private:
    void init_bricks();

    static coordinate get_init_paddle_coord();
    void handle_keyboard_input(sf::Time time);
    void check_paddle_collision(sf::Time time);
    void check_brick_collision(sf::Time time);
    void handle_ball_movement(sf::Time time);
    void start();

    game_window m_window;
    paddle m_paddle;
    std::shared_ptr<ball> m_p_ball;
    std::map<int, std::map<int, std::unique_ptr<brick>>> m_bricks {};
    bool m_started = false;

    int m_column_width = 0;
};

