#include "MainBossScreen.h"
#include <SFML/Graphics.hpp>
#include <string>

MainBossScreen::MainBossScreen(const sf::Font& fontRef)
    : font(fontRef), placeholderText(fontRef, "", 36) {}

void MainBossScreen::start(int floorNum) {
    floorNumber = floorNum;
    complete = false;
    std::string msg = "Main Boss Room (Floor " + std::to_string(floorNumber) + ")\n\n[Placeholder]\nClick to go to the next floor.";
    placeholderText.setString(msg);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setPosition(sf::Vector2f(200.f, 300.f));
}

void MainBossScreen::draw(sf::RenderWindow& window) {
    window.draw(placeholderText);
}

bool MainBossScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (complete) return false;
    if (event.is<sf::Event::MouseButtonPressed>() || event.is<sf::Event::KeyPressed>()) {
        complete = true;
        return true;
    }
    return false;
}

bool MainBossScreen::isComplete() const {
    return complete;
}
