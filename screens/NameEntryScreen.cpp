#include "NameEntryScreen.h"

NameEntryScreen::NameEntryScreen(const sf::Font& font)
    : font(font),
      prompt(font, "Enter your name:", 28),
      inputText(font, "", 28) {
    prompt.setPosition({100.f, 100.f});
    prompt.setFillColor(sf::Color::White);
    inputText.setPosition({110.f, 155.f});
    inputText.setFillColor(sf::Color::Yellow);
    nameInput = "";
    // Setup textbox
    textbox.setSize({textboxWidth, textboxHeight});
    textbox.setPosition({100.f, 150.f});
    textbox.setFillColor(sf::Color(20, 20, 30));
    textbox.setOutlineColor(sf::Color::White);
    textbox.setOutlineThickness(2.f);
    showCursor = true;
    cursorClock.restart();
}

bool NameEntryScreen::handleEvent(const sf::Event& event, std::string& playerName) {
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (textEvent->unicode == '\b') { // Backspace
            if (!nameInput.isEmpty()) nameInput.erase(nameInput.getSize() - 1, 1);
        } else if (textEvent->unicode == '\r' || textEvent->unicode == '\n') { // Enter
            if (!nameInput.isEmpty()) {
                playerName = nameInput;
                return true;
            }
        } else if (textEvent->unicode >= 32 && textEvent->unicode < 127 && nameInput.getSize() < 16) {
            nameInput += textEvent->unicode;
        }
        inputText.setString(nameInput);
    }
    return false;
}

void NameEntryScreen::draw(sf::RenderWindow& window) {
    window.draw(prompt);
    window.draw(textbox);
    window.draw(inputText);
    // Draw blinking cursor
    if (showCursor) {
        auto bounds = inputText.getLocalBounds();
        float x = inputText.getPosition().x + bounds.position.x + bounds.size.x + 2.f;
        float y = inputText.getPosition().y;
        sf::RectangleShape cursor({2.f, static_cast<float>(inputText.getCharacterSize())});
        cursor.setPosition({x, y});
        cursor.setFillColor(sf::Color::Yellow);
        window.draw(cursor);
    }
    // Blink logic
    if (cursorClock.getElapsedTime().asSeconds() > 0.5f) {
        showCursor = !showCursor;
        cursorClock.restart();
    }
}

void NameEntryScreen::reset() {
    nameInput = "";
    inputText.setString("");
}
