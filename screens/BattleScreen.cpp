// Required by ScreenBase, but not used for BattleScreen
#include "BattleScreen.h"
#include <SFML/Graphics.hpp>
#include "../AnimatedSprite.h"

void BattleScreen::start(int floorNum) {
    // Optionally, you could call your custom start with default args or leave empty
}

BattleScreen::BattleScreen(const sf::Font& font)
    : font(font), player("", "", 1, 1, 1, 1) {}

void BattleScreen::start(int floorNum, const Character& playerChar, AnimatedSprite* playerIdleAnimPtr, const std::vector<Character>& enemyChars, const std::vector<AnimatedSprite*>& enemyIdleAnimsVec) {
    floorNumber = floorNum;
    player = playerChar;
    enemies = enemyChars;
    enemyIdleAnims = enemyIdleAnimsVec;
    battleOver = false;
    introText = "You have met " + (enemies.empty() ? "an enemy!" : enemies[0].name + "!");
    introAlpha = 0.f;
    selectedOption = 0;
    playerIdleAnim = playerIdleAnimPtr;
}

void BattleScreen::draw(sf::RenderWindow& window) {
    drawCharacters(window);
    drawIntro(window);
    drawOptions(window);
}

void BattleScreen::drawCharacters(sf::RenderWindow& window) {
    // Draw player idle animation on left
    if (playerIdleAnim) {
        float px = 80.f + 64.f;
        float py = window.getSize().y / 2.f;
        static sf::Clock animClock;
        float dt = animClock.restart().asSeconds();
        playerIdleAnim->update(dt);
        playerIdleAnim->draw(window, sf::Vector2f(px, py));
    }
    // Draw enemies on right
    float enemySpacing = 160.f;
    for (size_t i = 0; i < enemyIdleAnims.size(); ++i) {
        AnimatedSprite* anim = enemyIdleAnims[i];
        float ex = window.getSize().x - 160.f - i * enemySpacing;
        float ey = window.getSize().y / 2.f;
        if (anim) {
            static sf::Clock animClock;
            float dt = animClock.restart().asSeconds();
            anim->update(dt);
            anim->draw(window, sf::Vector2f(ex, ey));
        } else {
            sf::CircleShape enemyShape(64.f);
            enemyShape.setFillColor(sf::Color(180, 80, 80));
            enemyShape.setPosition(sf::Vector2f(ex - 64.f, ey - 64.f));
            window.draw(enemyShape);
        }
    }
}

void BattleScreen::drawIntro(sf::RenderWindow& window) {
    if (introAlpha < 1.f) return;
    sf::Text intro(font, introText, 36);
    intro.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(introAlpha * 255)));
    auto bounds = intro.getLocalBounds();
    float textWidth = bounds.size.x;
    intro.setPosition(sf::Vector2f(window.getSize().x / 2.f - textWidth / 2.f, 80.f));
    window.draw(intro);
}

void BattleScreen::drawOptions(sf::RenderWindow& window) {
    float boxWidth = 320.f, boxHeight = 180.f;
    float x = window.getSize().x - boxWidth - 48.f;
    float y = window.getSize().y - boxHeight - 48.f;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(40, 40, 60, 220));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(3.f);
    box.setPosition(sf::Vector2f(x, y));
    window.draw(box);
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text opt(font, options[i], 32);
        opt.setFillColor(i == selectedOption ? sf::Color(180, 220, 255) : sf::Color::White);
        auto obounds = opt.getLocalBounds();
        float optWidth = obounds.size.x;
        opt.setPosition(sf::Vector2f(x + 32.f, y + 32.f + i * 48.f));
        window.draw(opt);
    }
}

void BattleScreen::updateIntro(float dt) {
    if (introAlpha < 1.f) {
        introAlpha += dt * 0.8f;
        if (introAlpha > 1.f) introAlpha = 1.f;
    }
}

bool BattleScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Simple option navigation
    // TODO: Check your SFML 3.x event/key API. The following is typical, but may need adjustment:
    // if (event.type == sf::Event::KeyPressed) {
    //     auto key = event.key.code;
    //     if (key == sf::Keyboard::Down) { ... }
    // }
    return false;
}

bool BattleScreen::isComplete() const {
    return battleOver;
}
