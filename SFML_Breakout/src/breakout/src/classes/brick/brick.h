#pragma once
#include <map>

#include "../drawable_object/drawable_object.h"
#include "../config/config.h"

constexpr sf::Color LOW_COLOR    = sf::Color::Green;
constexpr sf::Color MEDIUM_COLOR = sf::Color::Yellow;
constexpr sf::Color HIGH_COLOR   = sf::Color::Red;

struct ColorCompare
{
    bool operator() (const sf::Color& lhs, const sf::Color& rhs) const
    {
        return lhs.toInteger() < rhs.toInteger();
    }
};

class brick : public drawable_object{
public:
    brick() = delete;
    explicit brick(coordinate coord, float width, sf::Color color);
    [[nodiscard]] int get_score() const;

    brick &operator = (const brick &rhs) {
        if (this != &rhs) {
            auto renderable = new sf::RectangleShape();
            renderable->setSize({m_width, config::get_config_map().at("BRICK_HEIGHT")});
            m_renderable_element = std::unique_ptr<sf::RectangleShape>(renderable);
            m_renderable_element->setPosition(rhs.m_renderable_element->getPosition());
            m_renderable_element->setFillColor(rhs.m_renderable_element->getFillColor());
            m_color = rhs.m_color;
            m_width = rhs.m_width;
        }

        return *this;
    }
private:
    sf::Color m_color;
    float     m_width;
};
