#pragma once
#include <SFML/Graphics.hpp>

class ScreenBase {
public:
    virtual ~ScreenBase() = default;
    virtual void start(int floorNum) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool handleEvent(const sf::Event& event, sf::RenderWindow& window) = 0;
    virtual bool isComplete() const = 0;
};
