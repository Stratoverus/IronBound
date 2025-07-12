#include "SpriteSelectScreen.h"
#include "../AnimatedSprite.h"
#include <filesystem>
#include <iostream>

SpriteSelectScreen::SpriteSelectScreen(const sf::Font& font) : font(font), prompt(font, "Select Your Character:", 32) {
    prompt.setPosition({100.f, 80.f});
    prompt.setFillColor(sf::Color::White);
    // Scan assets/sprites/playable for character folders
    std::string baseDir = "assets/sprites/playable";
    std::vector<std::string> animationNames = {"Idle", "Attack", "Run", "Hit", "Death"}; // Add more as needed
    for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
        if (entry.is_directory()) {
            std::string charName = entry.path().filename().string();
            characterNames.push_back(charName);
            std::map<std::string, AnimatedSprite> animMap;
            for (const auto& animName : animationNames) {
                std::string animPath = (entry.path() / (animName + ".png")).string();
                if (std::filesystem::exists(animPath)) {
                    float frameTime = 0.18f;
                    animMap[animName] = AnimatedSprite({animPath}, frameTime);
                    animMap[animName].setScale({2.f, 2.f});
                }
            }
            characterAnimations.push_back(std::move(animMap));
        }
    }
}

bool SpriteSelectScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window, int& hovered, int& selectedIndex) {
    // Use the same box layout as draw()
    float screenWidth = window.getSize().x;
    float screenHeight = window.getSize().y;
    size_t num = characterAnimations.size();
    float desiredHeight = std::min(450.f, (screenHeight - 120.f) * 0.9f);
    float spacing = desiredHeight * 0.25f;
    float totalWidth = num * desiredHeight + (num - 1) * spacing;
    float leftStart = (screenWidth - totalWidth) / 2.f;
    float yPos = screenHeight / 2.0f;

    if (event.is<sf::Event::MouseMoved>()) {
        hovered = -1;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    for (size_t i = 0; i < characterAnimations.size(); ++i) {
        float xPos = leftStart + i * (desiredHeight + spacing) + desiredHeight / 2.0f;
        sf::FloatRect bounds({xPos - desiredHeight / 2.0f, yPos - desiredHeight / 2.0f}, {desiredHeight, desiredHeight});
        if (bounds.contains(window.mapPixelToCoords(mousePos))) {
            hovered = static_cast<int>(i);
        }
    }
    }
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            for (size_t i = 0; i < characterAnimations.size(); ++i) {
                float xPos = leftStart + i * (desiredHeight + spacing) + desiredHeight / 2.0f;
                sf::FloatRect bounds({xPos - desiredHeight / 2.0f, yPos - desiredHeight / 2.0f}, {desiredHeight, desiredHeight});
                if (bounds.contains(window.mapPixelToCoords(mousePos))) {
                    selectedIndex = static_cast<int>(i);
                    return true;
                }
            }
        }
    }
    return false;
}

void SpriteSelectScreen::draw(sf::RenderWindow& window, int hoveredIndex, float dt) {
    window.draw(prompt);
    float screenWidth = window.getSize().x;
    float screenHeight = window.getSize().y;
    size_t num = characterAnimations.size();
    // 50% bigger boxes, centered vertically
    float desiredHeight = std::min(450.f, (screenHeight - 120.f) * 0.9f); // 450px max, 90% of available height
    float spacing = desiredHeight * 0.25f;
    float totalWidth = num * desiredHeight + (num - 1) * spacing;
    float leftStart = (screenWidth - totalWidth) / 2.f;
    float yPos = screenHeight / 2.0f; // Centered vertically

    // --- Fix hover logic: recalculate hovered every frame ---
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    int trueHovered = -1;
    for (size_t i = 0; i < characterAnimations.size(); ++i) {
        float xPos = leftStart + i * (desiredHeight + spacing) + desiredHeight / 2.0f;
        sf::FloatRect bounds({xPos - desiredHeight / 2.0f, yPos - desiredHeight / 2.0f}, {desiredHeight, desiredHeight});
        if (bounds.contains(window.mapPixelToCoords(mousePos))) {
            trueHovered = static_cast<int>(i);
        }
    }

    for (size_t i = 0; i < characterAnimations.size(); ++i) {
        auto& animMap = characterAnimations[i];
        auto it = animMap.find("Idle");
        if (it == animMap.end()) continue;
        AnimatedSprite& preview = animMap["Idle"];
        // Scale so tallest frame matches desiredHeight * 0.7 (leave room for text)
        const auto& rects = preview.getFrameRects();
        float tallest = 0.f;
        for (int f = 0; f < preview.getFrameCount(); ++f) {
            auto rect = rects.empty() ? sf::IntRect({0,0},{128,128}) : rects[f];
            if (rect.size.y > tallest) tallest = static_cast<float>(rect.size.y);
        }
        float spriteArea = desiredHeight * 0.7f;
        float scale = spriteArea / tallest;
        preview.setScale({scale, scale});
        preview.setFrameTime((static_cast<int>(i) == trueHovered) ? 0.12f : 0.35f);
        preview.update(dt);
        float xPos = leftStart + i * (desiredHeight + spacing) + desiredHeight / 2.0f;
        // Draw selection box/button
        sf::RectangleShape box(sf::Vector2f(desiredHeight, desiredHeight));
        box.setPosition(sf::Vector2f(xPos - desiredHeight / 2.0f, yPos - desiredHeight / 2.0f));
        box.setFillColor((static_cast<int>(i) == trueHovered) ? sf::Color(60, 60, 100) : sf::Color(40, 40, 60));
        box.setOutlineThickness(4.f);
        box.setOutlineColor((static_cast<int>(i) == trueHovered) ? sf::Color(180, 180, 255) : sf::Color(80, 80, 120));
        window.draw(box);
        // Draw preview so feet are at bottom of box
        float spriteY = yPos + desiredHeight * 0.35f; // Center, but feet at bottom
        preview.draw(window, {xPos, spriteY});
        // Draw name inside box, just above bottom
        sf::Text nameText(font);
        std::string name = characterNames[i];
        if (!name.empty()) name[0] = toupper(name[0]);
        nameText.setString(name);
        nameText.setCharacterSize(32);
        nameText.setPosition({xPos - nameText.getLocalBounds().size.x / 2.f, yPos + desiredHeight / 2.0f - 40.f});
        nameText.setFillColor(sf::Color::White);
        window.draw(nameText);
    }
}

int SpriteSelectScreen::getNumCharacters() const {
    return static_cast<int>(characterAnimations.size());
}
