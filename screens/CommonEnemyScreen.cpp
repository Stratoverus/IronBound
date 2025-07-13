#include <vector>
#include <cstdlib>
#include <ctime>
#include "../core/Enemy.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "CommonEnemyScreen.h"

CommonEnemyScreen::CommonEnemyScreen(const sf::Font& font)
    : BattleScreen(font) {}


void CommonEnemyScreen::start(int floorNum) {
    // This override is unused; all setup is handled in main.cpp
}

Enemy CommonEnemyScreen::getRandomCommonEnemy(const std::string& jsonPath) {
    static std::vector<Enemy> allEnemies = Enemy::loadEnemiesFromJSON(jsonPath);
    std::vector<Enemy> commonEnemies;
    for (const auto& e : allEnemies) {
        if (e.tag == "common") commonEnemies.push_back(e);
    }
    if (commonEnemies.empty()) return Enemy();
    return commonEnemies[rand() % commonEnemies.size()];
}
