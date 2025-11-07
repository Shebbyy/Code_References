#include "ball.h"

#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>

#include "../config/config.h"

ball::ball(coordinate init_coord): drawable_object() {
    sf::CircleShape * ball = new sf::CircleShape();

    ball->setRadius(config::get_config_map().at("BALL_RADIUS"));
    ball->setFillColor(sf::Color::White);
    m_renderable_element = std::unique_ptr<sf::CircleShape>(ball);

    drawable_object::set_renderable_element_position(init_coord);
}

void ball::set_velocity(sf::Vector2f velocity) {
    m_velocity = velocity;
}

sf::Vector2f ball::get_velocity() const {
    return m_velocity;
}

void ball::move(sf::Time time, coordinate window_dimensions) {
    sf::Vector2f velocity = get_velocity() * time.asSeconds();

    sf::Vector2f ball_position = m_renderable_element->getPosition();
    ball_position += velocity;

    sf::FloatRect window_bounds = sf::FloatRect({0, 0}, {window_dimensions.get_x(), window_dimensions.get_y()});

    m_renderable_element->move(velocity);
    if (!did_collide_with_something(window_bounds, true)) {
        auto intersect = window_bounds.findIntersection(m_renderable_element->getGlobalBounds());

        float ball_radius = config::get_config_map().at("BALL_RADIUS");
        // check if bottom of window hit, if yes --> set velocity to 0 --> outside game handling then resets ball position, removes health and binds ball to paddle
        if (   intersect
            && get_position().get_y() >= window_dimensions.get_y() - ball_radius * 2
            && round(intersect->size.y) < ball_radius * 2) {
            set_velocity({0, 0});
        } else {
            adjust_ball_direction(intersect, true);
            m_renderable_element->move(-velocity);

            move(time, window_dimensions); // Call Move again with the new velocity
        }
    }
}

bool ball::did_collide_with_something(sf::FloatRect collision_box, bool inverted) const {
    auto intersect = collision_box.findIntersection(m_renderable_element->getGlobalBounds());

    if (!intersect) {
        return false;
    }

    if (inverted) {
        float ball_radius = config::get_config_map().at("BALL_RADIUS");
        return round(intersect->size.x) == ball_radius * 2
            && round(intersect->size.y) == ball_radius * 2;
    }

    return round(intersect->size.x) > 0
        || round(intersect->size.y) > 0;
}

void ball::adjust_ball_direction(std::optional<sf::Rect<float>> intersection, bool inverted) {
    sf::Vector2f new_velocity;
    if (!intersection) {
        new_velocity = get_velocity().componentWiseMul({-1, -1});
    } else {
        if (evaluate_collision_cond(intersection->size.x, inverted)) {
            new_velocity = get_velocity().componentWiseMul({-1, 1});
        }

        if (evaluate_collision_cond(intersection->size.y, inverted)) {
            new_velocity = get_velocity().componentWiseMul({1, -1});
        }
    }
    set_velocity(new_velocity);
}

bool ball::evaluate_collision_cond(float collision_dimension_size, bool inverted) {
    if (inverted) {
        float ball_radius = config::get_config_map().at("BALL_RADIUS");
        return round(collision_dimension_size) < ball_radius * 2;
    }

    return round(collision_dimension_size) > 0;
}
