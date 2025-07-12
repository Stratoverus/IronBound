#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "BattleScreen.h"

class CommonEnemy : public BattleScreen {
public:
    using BattleScreen::start;
    CommonEnemy(const sf::Font& font);
    void start(int floorNum) override;
};
