#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class NameEntryScreen {
public:
    NameEntryScreen(const sf::Font& font);
    // Handles events, returns true if name entry is complete
    bool handleEvent(const sf::Event& event, std::string& playerName);
    void draw(sf::RenderWindow& window);
    void reset();
private:
    const sf::Font& font;
    sf::Text prompt;
    sf::Text inputText;
    sf::String nameInput;
    sf::RectangleShape textbox;
    float textboxWidth = 350.f;
    float textboxHeight = 40.f;
    // For blinking cursor
    bool showCursor = true;
    sf::Clock cursorClock;
};
