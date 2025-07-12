#include "PopupMenu.h"

PopupMenu::PopupMenu(sf::Font& fontRef)
    : font(fontRef), hovered(-1), visible(false) {
    options = {"Resume", "Save Game", "Settings", "Abandon Run", "Save and Quit to Windows", "Exit Without Saving"};
    background.setFillColor(sf::Color(30, 30, 40, 245));
    background.setOutlineColor(sf::Color::White);
    background.setOutlineThickness(3.f);
    for (const auto& opt : options) {
        sf::Text txt(font, opt, 32);
        txt.setFillColor(sf::Color::White);
        optionTexts.push_back(txt);
    }
}

void PopupMenu::show() { visible = true; }
void PopupMenu::hide() { visible = false; hovered = -1; }
bool PopupMenu::isVisible() const { return visible; }

void PopupMenu::updateLayout(sf::RenderWindow& window) {
    float width = 520.f, height = 60.f * options.size() + 40.f;
    background.setSize(sf::Vector2f(width, height));
    background.setPosition(sf::Vector2f(window.getSize().x / 2.f - width / 2.f, window.getSize().y / 2.f - height / 2.f));
    for (size_t i = 0; i < optionTexts.size(); ++i) {
        optionTexts[i].setPosition(sf::Vector2f(background.getPosition().x + 40.f, background.getPosition().y + 30.f + i * 60.f));
        optionTexts[i].setFillColor(i == hovered ? sf::Color(180, 180, 255) : sf::Color::White);
    }
}

void PopupMenu::draw(sf::RenderWindow& window) {
    if (!visible) return;
    updateLayout(window);
    window.draw(background);
    for (auto& txt : optionTexts) window.draw(txt);
}

bool PopupMenu::handleEvent(const sf::Event& event, sf::RenderWindow& window, int& selected) {
    if (!visible) return false;
    if (event.is<sf::Event::MouseMoved>()) {
        hovered = -1;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        for (size_t i = 0; i < optionTexts.size(); ++i) {
            if (optionTexts[i].getGlobalBounds().contains(worldPos)) {
                hovered = static_cast<int>(i);
            }
        }
    }
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left && hovered != -1) {
            selected = hovered;
            hide();
            // If 'Exit Without Saving' is selected, exit immediately
            if (options[selected] == "Exit Without Saving") {
                exit(0);
            }
            return true;
        }
    }
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
        if (keyEvent->code == sf::Keyboard::Key::Escape) {
            hide();
            return false;
        }
    }
    return false;
}
