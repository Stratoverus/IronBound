#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class AnimatedSprite; // Forward declaration

class CharacterStatsScreen {
public:
    CharacterStatsScreen(const sf::Font& font);
    void setStats(const std::string& name, const std::string& className, int hp, int maxHp, int level, int xp, int xpForNext, int attack, int defense, int speed, const std::vector<std::string>& moves, AnimatedSprite* idleAnim, const std::vector<std::string>& inventory, const std::vector<std::string>& equipped);
    void show();
    void hide();
    bool isVisible() const;
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
private:
    bool visible = false;
    sf::Clock openClock;
    float minOpenTime = 0.2f; // seconds
    sf::RectangleShape overlay;
    sf::RectangleShape statsBox;
    sf::RectangleShape xpBarBg;
    sf::RectangleShape xpBarFill;
    std::unique_ptr<sf::Text> titleText;
    std::unique_ptr<sf::Text> nameText;
    std::unique_ptr<sf::Text> classText;
    std::unique_ptr<sf::Text> hpText;
    std::unique_ptr<sf::Text> levelText;
    std::unique_ptr<sf::Text> xpText;
    std::unique_ptr<sf::Text> atkText;
    std::unique_ptr<sf::Text> defText;
    std::unique_ptr<sf::Text> spdText;
    std::vector<std::unique_ptr<sf::Text>> moveTexts;
    std::vector<std::unique_ptr<sf::Text>> inventoryTexts;
    std::vector<std::unique_ptr<sf::Text>> equippedTexts;
    AnimatedSprite* idleAnim = nullptr;
    int xp = 0, xpForNext = 1;
    sf::Font font;
};
