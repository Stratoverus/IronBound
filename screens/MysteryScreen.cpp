#include "MysteryScreen.h"
#include <SFML/Graphics.hpp>
#include <string>

MysteryScreen::MysteryScreen(const sf::Font& fontRef)
    : font(fontRef), placeholderText(fontRef, "", 36) {}

void MysteryScreen::start(int floorNum) {
    floorNumber = floorNum;
    complete = false;
    std::string msg = "Mystery Room (Floor " + std::to_string(floorNumber) + ")\n\n[Placeholder]\nClick to go to the next floor.";
    placeholderText.setString(msg);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(sf::Vector2f(200.f, 300.f));
}

void MysteryScreen::draw(sf::RenderWindow& window) {
    window.draw(placeholderText);
}

bool MysteryScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (complete) return false;
    if (event.is<sf::Event::MouseButtonPressed>() || event.is<sf::Event::KeyPressed>()) {
        complete = true;
        return true;
    }
    return false;
}

bool MysteryScreen::isComplete() const {
    return complete;
}
