#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class AnimatedSprite {
public:
    AnimatedSprite();
    AnimatedSprite(const std::vector<std::string>& framePaths, float frameTime = 0.15f);
    void update(float dt);
    void draw(sf::RenderWindow& window, sf::Vector2f position);
    void setScale(sf::Vector2f scale);
    void setFrameTime(float time);
    int getFrameCount() const;
    const std::vector<sf::IntRect>& getFrameRects() const;
private:
    std::vector<sf::Texture> frames;
    sf::Sprite* sprite;
    std::vector<sf::IntRect> detectedFrameRects;
    float frameTime; // seconds per frame
    float timer;
    int currentFrame;
};
