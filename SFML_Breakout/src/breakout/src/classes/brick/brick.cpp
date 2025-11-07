//
// Created by Shebby on 19/03/2025.
//

#include "brick.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "../score/score.h"

brick::brick(const coordinate coord, const float width, const sf::Color color): drawable_object(), m_color(color), m_width(width) {
    sf::RectangleShape * brick = new sf::RectangleShape();

    brick->setSize({width, config::get_config_map().at("BRICK_HEIGHT")});
    brick->setFillColor(color);
    brick->setOutlineThickness(OUTLINE_WIDTH);
    brick->setOutlineColor(sf::Color::White);
    m_renderable_element = std::unique_ptr<sf::RectangleShape>(brick);

    drawable_object::set_renderable_element_position(coord);
}

int brick::get_score() const {
    return score::get_score_for_color(m_color);
}
