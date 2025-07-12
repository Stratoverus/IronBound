#pragma once
#include <SFML/Graphics.hpp>
#include "ScreenBase.h"
#include "../core/Character.h"
#include <string>
#include "../AnimatedSprite.h"
#include <vector>

class BattleScreen : public ScreenBase {
public:
    BattleScreen(const sf::Font& font);
    void start(int floorNum, const Character& player, AnimatedSprite* playerIdleAnimPtr, const std::vector<Character>& enemies, const std::vector<AnimatedSprite*>& enemyIdleAnims);
    void start(int floorNum) override;
    void draw(sf::RenderWindow& window) override;
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    bool isComplete() const override;
protected:
    const sf::Font& font;
    int floorNumber = 1;
    Character player;
    std::vector<Character> enemies;
    AnimatedSprite* playerIdleAnim = nullptr;
    std::vector<AnimatedSprite*> enemyIdleAnims;
    bool battleOver = false;
    std::string introText;
    float introAlpha = 0.f;
    int selectedOption = 0;
    std::vector<std::string> options = {"Fight", "Item", "Run"};
    void drawIntro(sf::RenderWindow& window);
    void drawOptions(sf::RenderWindow& window);
    void drawCharacters(sf::RenderWindow& window);
    void updateIntro(float dt);
};
