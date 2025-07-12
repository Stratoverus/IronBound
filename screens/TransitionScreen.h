#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class TransitionScreen {
public:
    TransitionScreen(const sf::Font& font);
    void setMessage(const std::string& message);
    void draw(sf::RenderWindow& window);
private:
    sf::Text flavorText;
};
