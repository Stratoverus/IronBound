#include "TransitionScreen.h"

TransitionScreen::TransitionScreen(const sf::Font& font)
    : flavorText(font, "", 40) // SFML 3.x: font, string, size
{
    flavorText.setFillColor(sf::Color::White);
}

void TransitionScreen::setMessage(const std::string& message) {
    flavorText.setString(message);
}

void TransitionScreen::draw(sf::RenderWindow& window) {
    // Center the text using .position and .size
    auto bounds = flavorText.getLocalBounds();
    flavorText.setOrigin(sf::Vector2f(bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f));
    flavorText.setPosition(sf::Vector2f(window.getSize().x / 2.f, window.getSize().y / 2.f));
    window.draw(flavorText);
}
