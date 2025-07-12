#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MenuScreen {
public:
    MenuScreen(sf::Font& font);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, int& hovered, int& selected);
    void draw(sf::RenderWindow& window, int hovered);
    std::vector<std::string> menuItems;
    std::vector<sf::Text> menuTexts;
};
