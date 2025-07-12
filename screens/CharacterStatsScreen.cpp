#include "CharacterStatsScreen.h"
#include "../AnimatedSprite.h"
#include <iostream>

CharacterStatsScreen::CharacterStatsScreen(const sf::Font& font) : font(font) {
    overlay.setFillColor(sf::Color(0, 0, 0, 120));
    statsBox.setFillColor(sf::Color(40, 40, 60, 255));
    statsBox.setOutlineColor(sf::Color::White);
    statsBox.setOutlineThickness(4.f);
    xpBarBg.setFillColor(sf::Color(60, 60, 100));
    xpBarBg.setOutlineColor(sf::Color::White);
    xpBarBg.setOutlineThickness(2.f);
    xpBarFill.setFillColor(sf::Color(80, 180, 255));
    titleText = std::make_unique<sf::Text>(font, "Character Stats", 32);
    titleText->setFillColor(sf::Color::White);
    nameText = std::make_unique<sf::Text>(font, "", 24);
    nameText->setFillColor(sf::Color::White);
    classText = std::make_unique<sf::Text>(font, "", 24);
    classText->setFillColor(sf::Color::White);
    hpText = std::make_unique<sf::Text>(font, "", 24);
    hpText->setFillColor(sf::Color::White);
    levelText = std::make_unique<sf::Text>(font, "", 24);
    levelText->setFillColor(sf::Color::White);
}

void CharacterStatsScreen::setStats(
    const std::string& name,
    const std::string& className,
    int hp, int maxHp,
    int level, int xp, int xpForNext,
    int attack, int defense, int speed,
    const std::vector<std::string>& moves,
    AnimatedSprite* idleAnim,
    const std::vector<std::string>& inventory,
    const std::vector<std::string>& equipped
) {
    // ...existing code...
    nameText->setString("Name: " + name);
    classText->setString("Class: " + className);
    hpText->setString("HP: " + std::to_string(hp) + " / " + std::to_string(maxHp));
    levelText->setString("Level: " + std::to_string(level));
    xpText = std::make_unique<sf::Text>(font, "XP: " + std::to_string(xp) + " / " + std::to_string(xpForNext), 22);
    xpText->setFillColor(sf::Color(200, 220, 255));
    atkText = std::make_unique<sf::Text>(font, "ATK: " + std::to_string(attack), 22);
    atkText->setFillColor(sf::Color(255, 180, 180));
    defText = std::make_unique<sf::Text>(font, "DEF: " + std::to_string(defense), 22);
    defText->setFillColor(sf::Color(180, 255, 180));
    spdText = std::make_unique<sf::Text>(font, "SPD: " + std::to_string(speed), 22);
    spdText->setFillColor(sf::Color(180, 220, 255));
    moveTexts.clear();
    for (const auto& move : moves) {
        auto moveText = std::make_unique<sf::Text>(font, "- " + move, 22);
        moveText->setFillColor(sf::Color(200, 220, 255));
        moveText->setPosition(sf::Vector2f(0.f, 0.f));
        moveTexts.push_back(std::move(moveText));
    }
    inventoryTexts.clear();
    for (const auto& item : inventory) {
        auto itemText = std::make_unique<sf::Text>(font, item, 20);
        itemText->setFillColor(sf::Color(255, 255, 200));
        inventoryTexts.push_back(std::move(itemText));
    }
    equippedTexts.clear();
    for (const auto& item : equipped) {
        auto eqText = std::make_unique<sf::Text>(font, item, 20);
        eqText->setFillColor(sf::Color(200, 255, 255));
        equippedTexts.push_back(std::move(eqText));
    }
    this->idleAnim = idleAnim;
    this->xp = xp;
    this->xpForNext = xpForNext;
}

void CharacterStatsScreen::show() {
    visible = true;
    openClock.restart();
    // ...existing code...
}
void CharacterStatsScreen::hide() {
    visible = false;
    // ...existing code...
}
bool CharacterStatsScreen::isVisible() const { return visible; }

void CharacterStatsScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (openClock.getElapsedTime().asSeconds() < minOpenTime) return;
    if (event.is<sf::Event::KeyPressed>()) {
        auto keyEvent = event.getIf<sf::Event::KeyPressed>();
        if (keyEvent->code == sf::Keyboard::Key::I || keyEvent->code == sf::Keyboard::Key::Escape) {
            hide();
        }
    }
    if (event.is<sf::Event::MouseButtonPressed>()) {
        hide();
    }
}

void CharacterStatsScreen::draw(sf::RenderWindow& window) {
    if (!visible) return;
    // ...existing code...
    sf::Vector2u winSize = window.getSize();
    float overlayW = winSize.x * 0.8f;
    float overlayH = winSize.y * 0.8f;
    float overlayX = winSize.x * 0.1f;
    float overlayY = winSize.y * 0.1f;
    overlay.setSize(sf::Vector2f(winSize.x, winSize.y));
    window.draw(overlay);
    statsBox.setSize(sf::Vector2f(overlayW, overlayH));
    statsBox.setPosition(sf::Vector2f(overlayX, overlayY));
    window.draw(statsBox);
    // Left: Idle animation
    float leftW = overlayW * 0.38f;
    float leftH = overlayH - 40.f;
    float leftX = overlayX + 20.f;
    float leftY = overlayY + 20.f;
    if (idleAnim) {
        idleAnim->setScale(sf::Vector2f(leftW / 200.f, leftH / 200.f)); // 200 is a reasonable base size
        sf::Vector2f animPos(leftX + leftW / 2.f, leftY + leftH / 2.f);
        static sf::Clock animClock;
        float dt = animClock.restart().asSeconds();
        idleAnim->update(dt); // Use elapsed time for smooth animation
        idleAnim->draw(window, animPos);
    }
    // Right: Stats
    float rightX = leftX + leftW + 24.f;
    float rightY = leftY;
    float statSpacing = 38.f;
    titleText->setPosition(sf::Vector2f(rightX, rightY));
    window.draw(*titleText);
    nameText->setPosition(sf::Vector2f(rightX, rightY + statSpacing));
    window.draw(*nameText);
    classText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 2));
    window.draw(*classText);
    levelText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 3));
    window.draw(*levelText);
    hpText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 4));
    window.draw(*hpText);
    atkText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 5));
    window.draw(*atkText);
    defText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 6));
    window.draw(*defText);
    spdText->setPosition(sf::Vector2f(rightX, rightY + statSpacing * 7));
    window.draw(*spdText);
    // XP bar
    float xpBarW = overlayW * 0.32f;
    float xpBarH = 22.f;
    float xpBarX = rightX;
    float xpBarY = rightY + statSpacing * 8 + 10.f;
    xpBarBg.setSize(sf::Vector2f(xpBarW, xpBarH));
    xpBarBg.setPosition(sf::Vector2f(xpBarX, xpBarY));
    window.draw(xpBarBg);
    float xpRatio = xpForNext > 0 ? std::min(1.f, float(xp) / float(xpForNext)) : 0.f;
    xpBarFill.setSize(sf::Vector2f(xpBarW * xpRatio, xpBarH));
    xpBarFill.setPosition(sf::Vector2f(xpBarX, xpBarY));
    window.draw(xpBarFill);
    xpText->setPosition(sf::Vector2f(xpBarX + 8.f, xpBarY - 2.f));
    window.draw(*xpText);
    // Moves
    float movesY = xpBarY + xpBarH + 18.f;
    for (size_t i = 0; i < moveTexts.size(); ++i) {
        moveTexts[i]->setPosition(sf::Vector2f(rightX, movesY + i * 28.f));
        window.draw(*moveTexts[i]);
    }
    // Equipped grid (top right)
    float eqGridX = overlayX + overlayW * 0.62f;
    float eqGridY = overlayY + overlayH * 0.18f;
    float boxSize = 48.f;
    int eqCols = 3, eqRows = 2;
    sf::RectangleShape eqBox;
    eqBox.setSize(sf::Vector2f(boxSize, boxSize));
    eqBox.setFillColor(sf::Color(80, 80, 120));
    eqBox.setOutlineColor(sf::Color(200, 255, 255));
    eqBox.setOutlineThickness(2.f);
    sf::Text eqLabel(font, "Equipped:", 22);
    eqLabel.setFillColor(sf::Color(200, 255, 255));
    eqLabel.setPosition(sf::Vector2f(eqGridX, eqGridY - 32.f));
    window.draw(eqLabel);
    for (int r = 0; r < eqRows; ++r) {
        for (int c = 0; c < eqCols; ++c) {
            float bx = eqGridX + c * (boxSize + 8.f);
            float by = eqGridY + r * (boxSize + 8.f);
            eqBox.setPosition(sf::Vector2f(bx, by));
            window.draw(eqBox);
            int idx = r * eqCols + c;
            if (idx < equippedTexts.size()) {
                equippedTexts[idx]->setPosition(sf::Vector2f(bx + 6.f, by + 12.f));
                window.draw(*equippedTexts[idx]);
            }
        }
    }
    // Inventory grid (bottom right)
    float invGridX = overlayX + overlayW * 0.62f;
    float invGridY = overlayY + overlayH * 0.55f;
    int cols = 5, rows = 2;
    sf::RectangleShape invBox;
    invBox.setSize(sf::Vector2f(boxSize, boxSize));
    invBox.setFillColor(sf::Color(60, 60, 80));
    invBox.setOutlineColor(sf::Color(180, 180, 220));
    invBox.setOutlineThickness(2.f);
    sf::Text invLabel(font, "Inventory:", 22);
    invLabel.setFillColor(sf::Color(255, 255, 200));
    invLabel.setPosition(sf::Vector2f(invGridX, invGridY - 32.f));
    window.draw(invLabel);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            float bx = invGridX + c * (boxSize + 8.f);
            float by = invGridY + r * (boxSize + 8.f);
            invBox.setPosition(sf::Vector2f(bx, by));
            window.draw(invBox);
            int idx = r * cols + c;
            if (idx < inventoryTexts.size()) {
                inventoryTexts[idx]->setPosition(sf::Vector2f(bx + 6.f, by + 12.f));
                window.draw(*inventoryTexts[idx]);
            }
        }
    }
    // ...bag section removed...
}
