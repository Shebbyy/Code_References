#pragma once

#include <SFML/System/Time.hpp>

#include "../drawable_object/drawable_object.h"

const float BALL_PADDLE_OFFSET = 75.0f;

const float MAX_BALL_ROTATION    = 45.0f;
const float UPWARDS_ANGLE = -90.0f;

class ball: public drawable_object {
public:
    ball() = delete;
    explicit ball(coordinate init_coord);

    void set_velocity(sf::Vector2f velocity);
    [[nodiscard]] sf::Vector2f get_velocity() const;
    void move(sf::Time time, coordinate window_dimensions);
    [[nodiscard]] bool did_collide_with_something(sf::FloatRect collision_box, bool inverted) const;
    void adjust_ball_direction(std::optional<sf::Rect<float>> intersection, bool inverted);
private:

    static bool evaluate_collision_cond(float collision_dimension_size, bool inverted);
    sf::Vector2f m_velocity;
};
