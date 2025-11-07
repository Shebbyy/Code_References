#include "config.h"

#include <fstream>
#include <iostream>

std::vector<sf::Color> config::rows(0, sf::Color::White);

std::map<std::string, float> config::get_config_map() {
    static std::map<std::string, float> config_map;
    if (config_map.empty()) {
        std::ifstream configFile("." + CONFIG_FILEPATH);

        if (!configFile.good()) {
            std::cerr << "Die Config-Datei \"" << CONFIG_FILEPATH << "\" konnte nicht ausgelesen werden!" << std::endl;
            exit(1);
        }

        rows.clear();
        while(configFile.good()) {
            std::string line;
            std::getline(configFile, line);

            // Ignore Config Comments
            if (line.starts_with("--")) {
                continue;
            }

            std::erase(line, ' ');

            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string name  = line.substr(0, pos);
                std::string value = line.erase(0, pos + 1);

                if (name != "BRICK_ROWS") {
                    config_map[name] = std::stof(value);
                } else {
                    pos = value.find(';');
                    while (pos != std::string::npos) {
                        std::string color = value.substr(0, pos);
                        value = value.erase(0, pos + 1);
                        rows.push_back(get_color(color));

                        pos = value.find(';');
                    }
                }
            }
        }

        configFile.close();

        config_map["Initialized"] = 1;
    }

    return config_map;
}

std::vector<sf::Color> config::get_configurated_rows() {
    return rows;
}

sf::Color config::get_color(const std::string& value) {
    // Switch not available for string, so if is used instead
    if (value == "RED") {
        return sf::Color::Red;
    }

    if (value == "YELLOW") {
        return sf::Color::Yellow;
    }

    if (value == "GREEN") {
        return sf::Color::Green;
    }

    std::cerr << "Für die Farbe " << value << " wurde kein Mapping definiert, bitte nur RED YELLOW oder GREEN mit ; getrennt verwenden!" << std::endl;
    exit(1);
}
