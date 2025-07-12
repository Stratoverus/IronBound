#include <vector>
#include <cstdlib>
#include <ctime>
#include "../core/Enemy.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "CommonEnemy.h"

CommonEnemy::CommonEnemy(const sf::Font& font)
    : BattleScreen(font) {}


void CommonEnemy::start(int floorNum) {
    // This override is unused; all setup is handled in main.cpp
}

Enemy CommonEnemy::getRandomCommonEnemy(const std::string& jsonPath) {
    static std::vector<Enemy> allEnemies = Enemy::loadEnemiesFromJSON(jsonPath);
    std::vector<Enemy> commonEnemies;
    for (const auto& e : allEnemies) {
        if (e.tag == "common") commonEnemies.push_back(e);
    }
    if (commonEnemies.empty()) return Enemy();
    return commonEnemies[rand() % commonEnemies.size()];
}
