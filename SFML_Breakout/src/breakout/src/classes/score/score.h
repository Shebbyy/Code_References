#pragma once
#include <SFML/Graphics/Text.hpp>

constexpr float SCORE_SIDE_PADDING = 5.0f;

class score {
public:
    static score &get_instance(bool force_recreate = false);

    static void reset();
    [[nodiscard]] sf::Text get_score_text() const;
    [[nodiscard]] sf::Text get_live_text() const;

    void add_to_score(int add);
    void remove_live();
    bool no_lives_left() const;
    int get_score() const;
    static int get_score_for_color(sf::Color color) ;

protected:
    score() = default;

    int m_score = 0;
    int m_lives = 0;
};
