#include <SFML/Graphics.hpp>
#include <string>
#include "CommonEnemy.h"

CommonEnemy::CommonEnemy(const sf::Font& font)
    : BattleScreen(font) {}

void CommonEnemy::start(int floorNum) {
    // This override is unused; all setup is handled in main.cpp
}
