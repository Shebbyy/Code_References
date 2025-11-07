#include "paddle.h"

#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

#include "../breakout_game/breakout_game.h"

paddle::paddle(coordinate init_coord): drawable_object() {
   sf::RectangleShape * paddle = new sf::RectangleShape();

   paddle->setSize(get_dimensions());
   paddle->setFillColor(sf::Color::Cyan);
   m_renderable_element = std::unique_ptr<sf::RectangleShape>(paddle);

   paddle::set_renderable_element_position(init_coord);
}

sf::Vector2f paddle::get_dimensions() {
   auto config_map = config::get_config_map();
   return {config_map.at("PADDLE_WIDTH"), config_map.at("PADDLE_HEIGHT")};
}

void paddle::add_bound_ball(const std::shared_ptr<ball> &p_ball) {
   m_p_ball = p_ball;
}

void paddle::unbind_ball() {
   if (m_p_ball) {
      m_p_ball = nullptr;
   }
}

void paddle::set_renderable_element_position(coordinate coord) const {
   drawable_object::set_renderable_element_position(coord);
}

void paddle::move(sf::Vector2<float> offset) {
   m_renderable_element->move(offset);

   if (m_p_ball) {
      m_p_ball->set_renderable_element_position(breakout_game::get_init_ball_coord(get_position()));
   }
}
