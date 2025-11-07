#include "breakout_game.h"

#include <iostream>
#include <memory>

#include "../score/score.h"

breakout_game::breakout_game()
    : m_paddle(get_init_paddle_coord())
    , m_p_ball(std::make_shared<ball>(get_init_ball_coord(m_paddle.get_position()))) {
    config::get_config_map();
    score::reset();

    m_paddle.add_bound_ball(m_p_ball);
    init_bricks();

    sf::Clock clock;
    float speedup_timer_counter = 0;
    while (m_window.isOpen()) {
        // Update the circle's position
        sf::Time delta_time = clock.restart();
        handle_keyboard_input(delta_time);
        if (m_started) {
            speedup_timer_counter += delta_time.asSeconds();

            handle_ball_movement(delta_time);

            if (speedup_timer_counter > 10) {
                speedup_timer_counter = 0;
                float speedup_factor = config::get_config_map().at("SPEED_INCREASE_FACTOR");
                m_p_ball->set_velocity(m_p_ball->get_velocity().componentWiseMul({speedup_factor, speedup_factor}));
            }
        }

        score game_score = score::get_instance();
        if (   m_bricks.empty()
            || game_score.no_lives_left()) {
            m_started = false;

            m_window.close();

            std::cout << "Game Over!" << std::endl;
            std::cout << "Total Score: " << game_score.get_score() << std::endl;

            return;
        }

        m_window.render(m_paddle, *m_p_ball, m_bricks);
    }
}

// therefore after the window opened the player can position the paddle and the ball before the game starts
void breakout_game::start() {
    m_paddle.unbind_ball();

    m_p_ball->set_velocity({0, 350});
}

void breakout_game::init_bricks() {
    float height = static_cast<float>(game_window::get_top_offset());
    auto config_map = config::get_config_map();
    int brick_columns = static_cast<int>(config_map.at("BRICK_COLUMNS"));
    m_column_width = static_cast<int>(config_map.at("WINDOW_WIDTH")) / brick_columns;

    int rowCount = 0;
    for (auto color : config::get_configurated_rows()) {
        for (int col = 0; col < brick_columns; col++) {
            m_bricks[rowCount][col] = std::unique_ptr<brick>(new brick(coordinate(static_cast<float>(col * m_column_width), height), static_cast<float>(m_column_width), color));
        }
        rowCount++;
        height += config_map.at("BRICK_HEIGHT");
    }
}

coordinate breakout_game::get_init_paddle_coord() {
    static coordinate cached_coord(-1, -1);

    if (   cached_coord.get_x() == -1
        && cached_coord.get_y() == -1) {
        coordinate window_dims = game_window::get_window_dimensions();
        sf::Vector2f paddle_dims = paddle::get_dimensions();

        coordinate paddle_coords(window_dims.get_x() / 2 - paddle_dims.x / 2,
                                 window_dims.get_y() - paddle_dims.y - PADDLE_BOTTOM_OFFSET);
        cached_coord = paddle_coords;
    }

    return cached_coord;
}

coordinate breakout_game::get_init_ball_coord(coordinate paddle_coord) {
    coordinate ball_coord = paddle_coord;

    float y = ball_coord.get_y();
    y -= BALL_PADDLE_OFFSET;
    ball_coord.set_y(y);

    auto config_map = config::get_config_map();
    float x = ball_coord.get_x();
    x += (config_map.at("PADDLE_WIDTH") / 2) - config_map.at("BALL_RADIUS");
    ball_coord.set_x(x);

    return ball_coord;
}

void breakout_game::handle_keyboard_input(sf::Time time) {
    sf::Vector2f velocity({0, 0});

    static float paddle_move_velocity = config::get_config_map().at("PADDLE_MOVE_VELOCITY");
    // Check if arrow key is pressed
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left)) {
        velocity.x -= paddle_move_velocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right)) {
        velocity.x += paddle_move_velocity;
    }
    if (   sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)
        && !m_started) {
        m_started = true;
        start();
    }

    sf::Vector2<float> paddle_offset = velocity * time.asSeconds();

    auto config_map = config::get_config_map();
    if (   m_paddle.get_position().get_x() + paddle_offset.x > 0
        && m_paddle.get_position().get_x() + paddle_offset.x + config_map.at("PADDLE_WIDTH") < config_map.at("WINDOW_WIDTH")) {
        m_paddle.move(paddle_offset);
    }
}

void breakout_game::check_paddle_collision(sf::Time time) {
    if (m_p_ball->did_collide_with_something(m_paddle.get_renderable_element()->getGlobalBounds(), false)) {
        m_p_ball->get_renderable_element()->move(-m_p_ball->get_velocity() * time.asSeconds());
        auto collision_box = m_paddle.get_renderable_element()->getGlobalBounds().findIntersection(m_p_ball->get_renderable_element()->getGlobalBounds());

        m_p_ball->adjust_ball_direction(collision_box, false);

        auto config_map = config::get_config_map();
        float paddle_width = config_map.at("PADDLE_WIDTH");

        float ball_x = m_p_ball->get_position().get_x() + config_map.at("BALL_RADIUS");
        float paddle_x = m_paddle.get_position().get_x() + paddle_width / 2;
        ball_x = ball_x - paddle_x;
        ball_x /= paddle_width / 2;
        sf::Vector2f velocity = m_p_ball->get_velocity();
        float angleOffset = UPWARDS_ANGLE - velocity.angle().wrapUnsigned().asDegrees();

        velocity = velocity.rotatedBy(sf::degrees(angleOffset));

        velocity = velocity.rotatedBy(sf::degrees(MAX_BALL_ROTATION * ball_x));

        m_p_ball->set_velocity(velocity);
    }
}

void breakout_game::check_brick_collision(sf::Time time) {
    coordinate position = m_p_ball->get_position();
    int rows = m_bricks.rbegin()->first + 1; // 1 more for some padding in the check
    int brick_height = static_cast<int>(config::get_config_map().at("BRICK_HEIGHT"));

    // if position of ball far below bricks dont even start checking
    if (position.get_y() > static_cast<float>(game_window::get_top_offset() + brick_height * rows)) {
        return;
    }

    // convert screen position to brick grid position
    int pos_col = static_cast<int>(position.get_x() / (float)m_column_width) + 1;
    int pos_row = std::min(static_cast<int>((position.get_y() - game_window::get_top_offset()) / brick_height), static_cast<int>(m_bricks.size()));

    // check surrounding rows
    for (int row = pos_row - 1; row <= pos_row + 1; row++) {
        if (!m_bricks.contains(row)) {
            continue;
        }

        // check surrounding cols of row
        for (int col = pos_col - 1; col <= pos_col + 1; col++) {
            if (   !m_bricks[row].contains(col)
                || !m_p_ball->did_collide_with_something(m_bricks[row][col]->get_renderable_element()->getGlobalBounds(), false)) {
                continue;
            }

            m_p_ball->adjust_ball_direction(m_bricks[row][col]->get_renderable_element()->getGlobalBounds().findIntersection(m_p_ball->get_renderable_element()->getGlobalBounds()), false);
            score::get_instance().add_to_score(m_bricks[row][col]->get_score());
            m_bricks[row].erase(col);

            if (m_bricks[row].empty()) {
                m_bricks.erase(row);
            }

            m_p_ball->get_renderable_element()->move((m_p_ball->get_velocity() * time.asSeconds()) * 4.0f);
            return;
        }
    }
}

void breakout_game::handle_ball_movement(sf::Time time) {
    m_p_ball->move(time, game_window::get_window_dimensions());

    // Collision with bottom of window --> Reset Ball
    if (m_p_ball->get_velocity() == sf::Vector2f(0, 0)) {
        m_paddle.add_bound_ball(m_p_ball);
        m_p_ball->set_renderable_element_position(get_init_ball_coord(m_paddle.get_position()));
        score::get_instance().remove_live();
        m_started = false;
        return;
    }

    check_paddle_collision(time);

    // Brick Collision
    check_brick_collision(time);
}
