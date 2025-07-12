#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "ScreenBase.h"

class MainBossScreen : public ScreenBase {
public:
    MainBossScreen(const sf::Font& font);
    void start(int floorNum) override;
    void draw(sf::RenderWindow& window) override;
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    bool isComplete() const override;
    static Enemy getRandomBossEnemy(const std::string& jsonPath = "enemies.json");
private:
    sf::Font font;
    sf::Text placeholderText;
    bool complete = false;
    int floorNumber = 1;
};
