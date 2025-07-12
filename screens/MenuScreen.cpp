#include "MenuScreen.h"
#include <SFML/Graphics/Text.hpp>

MenuScreen::MenuScreen(sf::Font& font) {
    menuItems = {"Start New Game", "View Battle Log", "Exit"};
    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text text(font, menuItems[i], 32);
        text.setPosition({100.f, 100.f + static_cast<float>(i) * 60.f});
        text.setFillColor(sf::Color::White);
        menuTexts.push_back(text);
    }
}

void MenuScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window, int& hovered, int& selected) {
    if (event.is<sf::Event::MouseMoved>()) {
        hovered = -1;
        for (size_t i = 0; i < menuTexts.size(); ++i) {
            if (menuTexts[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                hovered = static_cast<int>(i);
            }
        }
    }
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            for (size_t i = 0; i < menuTexts.size(); ++i) {
                if (menuTexts[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                    selected = static_cast<int>(i);
                }
            }
        }
    }
}

void MenuScreen::draw(sf::RenderWindow& window, int hovered) {
    for (size_t i = 0; i < menuTexts.size(); ++i) {
        if (static_cast<int>(i) == hovered) menuTexts[i].setFillColor(sf::Color::Yellow);
        else menuTexts[i].setFillColor(sf::Color::White);
        window.draw(menuTexts[i]);
    }
}
