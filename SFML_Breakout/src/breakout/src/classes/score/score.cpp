#include "score.h"

#include <SFML/Graphics/Font.hpp>

#include "../config/config.h"
#include "../game_window/game_window.h"

score &score::get_instance(bool force_recreate) {
    static score singleton;
    if (force_recreate) {
        singleton = score();
        singleton.m_lives = static_cast<int>(config::get_config_map().at("MAX_LIVES"));
    }

    return singleton;
}

void score::reset() {
    get_instance(true);
}

sf::Text score::get_score_text() const {
    static sf::Font font = sf::Font("./resources/Roboto.ttf");
    sf::Text score_text = {font, std::string("Score: ") + std::to_string(m_score)};
    score_text.setFillColor(sf::Color::White);
    score_text.setPosition({SCORE_SIDE_PADDING, 0});

    return score_text;
}

sf::Text score::get_live_text() const {
    static sf::Font font = sf::Font("./resources/Roboto.ttf");
    sf::Text life_text = {font, std::string("Lives: ") + std::string(m_lives, '*')};
    life_text.setFillColor(sf::Color::White);
    life_text.setPosition({config::get_config_map().at("WINDOW_WIDTH") - life_text.getGlobalBounds().size.x - SCORE_SIDE_PADDING, 0});

    return life_text;
}

void score::add_to_score(int add) {
    m_score += add;
}

void score::remove_live() {
    m_lives -= 1;
}

bool score::no_lives_left() const {
    return m_lives == 0;
}

int score::get_score() const {
    return m_score;
}

int score::get_score_for_color(sf::Color color) {
    static std::map<sf::Color, int, ColorCompare> color_map = {};
    if (color_map.empty()) {
        auto config_map = config::get_config_map();
        color_map[LOW_COLOR]    = static_cast<int>(config_map.at("LOW_SCORE"));
        color_map[MEDIUM_COLOR] = static_cast<int>(config_map.at("MEDIUM_SCORE"));
        color_map[HIGH_COLOR]   = static_cast<int>(config_map.at("HIGH_SCORE"));
    }

    return color_map.at(color);
}
