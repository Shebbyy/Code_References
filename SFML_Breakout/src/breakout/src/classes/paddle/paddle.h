#pragma once

#include <SFML/Graphics/Shape.hpp>

#include "../drawable_object/drawable_object.h"

#include "../ball/ball.h"

const float PADDLE_BOTTOM_OFFSET = 50;

class paddle : public drawable_object {
public:
    paddle() = delete;
    explicit paddle(coordinate init_coord);
    static sf::Vector2f get_dimensions();
    void add_bound_ball(const std::shared_ptr<ball> &p_ball);
    void unbind_ball();
    void set_renderable_element_position(coordinate coord) const override;
    void move(sf::Vector2<float> offset);

private:
    std::shared_ptr<ball> m_p_ball;
};