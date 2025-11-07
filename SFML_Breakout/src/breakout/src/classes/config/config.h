#pragma once
#include <map>
#include <string>
#include <vector>
#include <SFML/Graphics/Color.hpp>

const std::string CONFIG_FILEPATH = "/resources/gameconfig.cfg";

/**
 * Whats available in the config file:
 * - Ball-Radius
 * - Window-Height
 * - Window-Width
 * - Paddle-Move-Velocity
 * - Paddle Height
 * - Brick-Column-Count
 * - Brick-Rows
 * - Speed-Increase-Factor
 * - Lives
 * - BrickTypeScores
 * - Brick-Height
 */
class config {
public:
    config() = delete;
    static std::map<std::string, float> get_config_map();
    static std::vector<sf::Color> get_configurated_rows();

private:
    static sf::Color get_color(const std::string& value);

    static std::vector<sf::Color> rows;
};
