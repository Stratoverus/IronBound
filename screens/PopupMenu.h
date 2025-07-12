#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PopupMenu {
public:
    PopupMenu(sf::Font& font);
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window, int& selected);
    void show();
    void hide();
    bool isVisible() const;
private:
    sf::Font& font;
    std::vector<std::string> options;
    std::vector<sf::Text> optionTexts;
    sf::RectangleShape background;
    int hovered;
    bool visible;
    void updateLayout(sf::RenderWindow& window);
};
