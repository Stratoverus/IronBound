#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>

class DoorSelectionScreen {
    // Store window size for dynamic layout
public:
    DoorSelectionScreen(sf::Font& font);
    void setWindowSize(sf::Vector2u size) { windowSize = size; }
    sf::Vector2u windowSize = sf::Vector2u(1280, 720);
    void start(int floorNum); // Call after flavor text, pass floor number
    void draw(sf::RenderWindow& window);
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window, int& doorSelected);
    int getNumDoors() const { return numDoors; }
    std::vector<std::string> getDoorLabels() const;
private:
    int numDoors;
    int hovered;
    int floorNumber;
    sf::Font& font;
    std::vector<sf::RectangleShape> doorRects;
    std::vector<sf::Text> doorTexts;
    sf::Text floorText;
    void generateDoors();
};
