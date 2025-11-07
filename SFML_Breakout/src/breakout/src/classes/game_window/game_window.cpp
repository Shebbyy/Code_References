#include "game_window.h"

#include "../score/score.h"
#include "../config/config.h"

game_window::game_window()
    : m_window(sf::VideoMode({static_cast<unsigned>(config::get_config_map().at("WINDOW_WIDTH")), static_cast<unsigned>(config::get_config_map().at("WINDOW_HEIGHT"))}), WINDOW_TITLE, sf::Style::Default, sf::State::Windowed, get_window_settings()) {}

bool game_window::isOpen() const {
    return m_window.isOpen();
}

void game_window::render(paddle &player_paddle, ball &player_ball, std::map<int, std::map<int, std::unique_ptr<brick>>> &brick_map) {
    // Event processing
    while (const std::optional event = m_window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
    }

    // Render the circle
    m_window.clear();

    // Takes the elements which are given from the game to the window and draws them on the window
    draw_window_elements(player_paddle, player_ball, brick_map);

    // finally displays the contents of the window
    m_window.display();
}

void game_window::close() {
    m_window.close();
}

coordinate game_window::get_window_dimensions() {
    return coordinate({config::get_config_map().at("WINDOW_WIDTH"), config::get_config_map().at("WINDOW_HEIGHT")});
}

sf::ContextSettings game_window::get_window_settings() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8; // Request 8x antialiasing
    return settings;
}

void game_window::draw_window_elements(paddle &player_paddle, ball& player_ball, std::map<int, std::map<int, std::unique_ptr<brick>>> &brick_map) {
    m_window.draw(*player_paddle.get_renderable_element());
    m_window.draw(*player_ball.get_renderable_element());

    for (auto &brick_pair: brick_map) {
        for (auto &brick : brick_pair.second) {
            m_window.draw(*brick.second->get_renderable_element());
        }
    }

    m_window.draw(score::get_instance().get_score_text());
    m_window.draw(score::get_instance().get_live_text());
}

int game_window::get_top_offset() {
    return config::get_config_map().at("WINDOW_HEIGHT") / TOP_OFFSET_DIVISOR;
}