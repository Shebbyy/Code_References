#pragma once
#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "../game_window/coordinate/coordinate.h"

const float OUTLINE_WIDTH = 2.5f;

class drawable_object {
public:
    virtual ~drawable_object() = default;
    std::unique_ptr<sf::Shape> &get_renderable_element() {
        return m_renderable_element;
    }

    [[nodiscard]] coordinate get_position() const {
        return {m_renderable_element->getPosition().x, m_renderable_element->getPosition().y};
    }

    virtual void set_renderable_element_position(coordinate coord) const {m_renderable_element->setPosition({coord.get_x(), coord.get_y()});}

protected:
    drawable_object() = default;

    std::unique_ptr<sf::Shape> m_renderable_element;
};
