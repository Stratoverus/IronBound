#include "TreasureScreen.h"
#include <SFML/Graphics.hpp>
#include <string>

TreasureScreen::TreasureScreen(const sf::Font& fontRef)
    : font(fontRef), placeholderText(fontRef, "", 36) {}

void TreasureScreen::start(int floorNum) {
    floorNumber = floorNum;
    complete = false;
    std::string msg = "Treasure Room (Floor " + std::to_string(floorNumber) + ")\n\n[Placeholder]\nClick to go to the next floor.";
    placeholderText.setString(msg);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(sf::Vector2f(200.f, 300.f));
}

void TreasureScreen::draw(sf::RenderWindow& window) {
    window.draw(placeholderText);
}

bool TreasureScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (complete) return false;
    if (event.is<sf::Event::MouseButtonPressed>() || event.is<sf::Event::KeyPressed>()) {
        complete = true;
        return true;
    }
    return false;
}

bool TreasureScreen::isComplete() const {
    return complete;
}
