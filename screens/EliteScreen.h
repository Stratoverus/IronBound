#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ScreenBase.h"

class EliteScreen : public ScreenBase {
public:
    EliteScreen(const sf::Font& font);
    void start(int floorNum) override;
    void draw(sf::RenderWindow& window) override;
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    bool isComplete() const override;
private:
    sf::Font font;
    sf::Text placeholderText;
    bool complete = false;
    int floorNumber = 1;
};
