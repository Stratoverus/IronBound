#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include "../AnimatedSprite.h"

class SpriteSelectScreen {
public:
    SpriteSelectScreen(const sf::Font& font);
    // Returns true if a sprite is selected, sets selectedIndex
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window, int& hovered, int& selectedIndex);
    void draw(sf::RenderWindow& window, int hoveredIndex, float dt);
    int getNumCharacters() const;
    std::vector<std::string> characterNames;
    // Expose animatedPreviews for stats screen idle animation
    // For each character, map animation name to AnimatedSprite
    std::vector<std::map<std::string, AnimatedSprite>> characterAnimations;
private:
    const sf::Font& font;
    std::vector<std::string> previewPaths;
    sf::Text prompt;
};
