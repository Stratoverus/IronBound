#include "../core/Enemy.h"
#include "MiniBossScreen.h"
#include <SFML/Graphics.hpp>
#include <string>

MiniBossScreen::MiniBossScreen(const sf::Font& fontRef)
    : font(fontRef), placeholderText(fontRef, "", 36) {}

void MiniBossScreen::start(int floorNum) {
    floorNumber = floorNum;
    complete = false;
    std::string msg = "Mini-Boss Room (Floor " + std::to_string(floorNumber) + ")\n\n[Placeholder]\nClick to go to the next floor.";
    placeholderText.setString(msg);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(sf::Vector2f(200.f, 300.f));
}

void MiniBossScreen::draw(sf::RenderWindow& window) {
    window.draw(placeholderText);
}

bool MiniBossScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (complete) return false;
    if (event.is<sf::Event::MouseButtonPressed>() || event.is<sf::Event::KeyPressed>()) {
        complete = true;
        return true;
    }
    return false;
}

bool MiniBossScreen::isComplete() const {
    return complete;
}

Enemy MiniBossScreen::getRandomMiniBossEnemy(const std::string& jsonPath) {
    static std::vector<Enemy> allEnemies = Enemy::loadEnemiesFromJSON(jsonPath);
    std::vector<Enemy> filtered;
    for (const auto& e : allEnemies) {
        if (e.tag == "miniboss") filtered.push_back(e);
    }
    if (filtered.empty()) return Enemy();
    return filtered[rand() % filtered.size()];
}
