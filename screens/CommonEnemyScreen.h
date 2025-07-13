#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "BattleScreen.h"

class CommonEnemyScreen : public BattleScreen {
public:
    using BattleScreen::start;
    CommonEnemyScreen(const sf::Font& font);
    void start(int floorNum) override;

    // Utility to get a random common enemy from JSON
    static Enemy getRandomCommonEnemy(const std::string& jsonPath = "enemies.json");
};
