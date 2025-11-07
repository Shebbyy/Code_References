#pragma once
#include <map>
#include <SFML/Graphics.hpp>

#include "coordinate/coordinate.h"
#include "../ball/ball.h"
#include "../paddle/paddle.h"
#include "../brick/brick.h"

const sf::View    WORLD_VIEW{{0, 0}, {1000, 1000}};
const std::string WINDOW_TITLE  = "Breakout";
constexpr int     TOP_OFFSET_DIVISOR = 8;

class game_window {
public:
    game_window();
    bool isOpen() const;
    void render(paddle& player_paddle, ball& player_ball, std::map<int, std::map<int, std::unique_ptr<brick>>>& brick_map);
    void close();
    static coordinate get_window_dimensions();
    static int get_top_offset();

private:
    static sf::ContextSettings get_window_settings();

    void draw_window_elements(paddle& player_paddle, ball& player_ball, std::map<int, std::map<int, std::unique_ptr<brick>>>& brick_map);

    sf::RenderWindow m_window;
};
