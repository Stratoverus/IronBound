// Required by ScreenBase, but not used for BattleScreen
#include "../core/Enemy.h"
#include "BattleScreen.h"
#include <SFML/Graphics.hpp>
#include "../AnimatedSprite.h"

void BattleScreen::start(int floorNum) {
    // Optionally, you could call your custom start with default args or leave empty
}


BattleScreen::BattleScreen(const sf::Font& font)
    : font(font), player(), introTextObj(font) {
    introTextObj.setCharacterSize(36);
    introTextObj.setFillColor(sf::Color(255, 255, 255, 0));
    introState = IntroState::Done;
    // Initialize options box state variables
    showMoves = false;
    showItemsPopup = false;
    defendActive = false;
    runAttempted = false;
    runSuccess = false;
    hoveredOption = -1;
    // No placeholder moves/items; will use player's actual inventory
    lastActionLabel = "";
}

void BattleScreen::start(int floorNum, const Character& playerChar, AnimatedSprite* playerIdleAnimPtr, const std::vector<Character>& enemyChars, const std::vector<AnimatedSprite*>& enemyIdleAnimsVec) {
    floorNumber = floorNum;
    player = playerChar;
    // No moves/items at game start; clear popups
}

void BattleScreen::start(int floorNum, const Character& playerChar, AnimatedSprite* playerIdleAnimPtr, const std::vector<Enemy>& enemyChars, const std::vector<AnimatedSprite*>& enemyIdleAnimsVec) {
    floorNumber = floorNum;
    player = playerChar;
    // Equipped moves are now populated during character loading, not here.
    enemies = enemyChars;
    enemyIdleAnims = enemyIdleAnimsVec;
    battleOver = false;
    introText = "You have met " + (enemies.empty() ? "an enemy!" : enemies[0].name + "!");
    setIntroText(introText);
    selectedOption = 0;
    playerIdleAnim = playerIdleAnimPtr;
    // No moves/items at game start; clear popups
    // Initialize animated health values
    displayedPlayerHealth = player.health;
    displayedEnemyHealth.clear();
    for (const auto& enemy : enemies) {
        displayedEnemyHealth.push_back(enemy.health);
    }
    lastActionLabel = "";
}

void BattleScreen::draw(sf::RenderWindow& window) {
    drawCharacters(window);
    drawIntro(window);
    drawOptions(window);
    // Draw turn indicator at top center
    std::string turnText = (turnState == TurnState::Player) ? "Player Turn" : "Enemy Turn";
    sf::Text turnIndicator(font, turnText, 36);
    turnIndicator.setFillColor(turnState == TurnState::Player ? sf::Color(180,220,255) : sf::Color(255,120,120));
    auto turnBounds = turnIndicator.getLocalBounds();
    float turnX = window.getSize().x / 2.f - turnBounds.size.x / 2.f;
    float turnY = 32.f;
    turnIndicator.setPosition(sf::Vector2f(turnX, turnY));
    window.draw(turnIndicator);
    // Draw last action label near bottom, with box and shadow
    if (!lastActionLabel.empty()) {
        sf::Text actionText(font, lastActionLabel, 36);
        actionText.setFillColor(sf::Color(255, 255, 180));
        auto bounds = actionText.getLocalBounds();
        float textWidth = bounds.size.x;
        float textHeight = bounds.size.y;
        float boxPaddingX = 48.f;
        float boxPaddingY = 24.f;
        float boxWidth = textWidth + boxPaddingX * 2;
        float boxHeight = textHeight + boxPaddingY * 2;
        float winWidth = window.getSize().x;
        float winHeight = window.getSize().y;
        float boxX = winWidth / 2.f - boxWidth / 2.f;
        float boxY = winHeight - boxHeight - 64.f;
        // Draw shadow
        float shadowOffset = 10.f;
        sf::RectangleShape shadow(sf::Vector2f(boxWidth + shadowOffset * 2, boxHeight + shadowOffset * 2));
        shadow.setFillColor(sf::Color(0, 0, 0, 100));
        shadow.setPosition(sf::Vector2f(boxX - shadowOffset, boxY - shadowOffset));
        window.draw(shadow);
        // Draw main box
        sf::RectangleShape bgBox(sf::Vector2f(boxWidth, boxHeight));
        bgBox.setFillColor(sf::Color(40, 40, 60, 220));
        bgBox.setOutlineColor(sf::Color::White);
        bgBox.setOutlineThickness(6.f);
        bgBox.setPosition(sf::Vector2f(boxX, boxY));
        window.draw(bgBox);
        // Center text in box
        actionText.setPosition(sf::Vector2f(boxX + boxPaddingX, boxY + boxPaddingY));
        window.draw(actionText);
    }
    // Handle player move damage delay
    if (playerMovePending && playerMoveDelayClock.getElapsedTime().asSeconds() >= playerMoveDelaySeconds) {
        auto equippedMoves = player.getEquippedMoves();
        if (!enemies.empty() && pendingPlayerMoveIdx >= 0 && pendingPlayerMoveIdx < (int)equippedMoves.size()) {
            enemies[0].health -= equippedMoves[pendingPlayerMoveIdx].damage;
            if (enemies[0].health < 0) enemies[0].health = 0;
        }
        playerMovePending = false;
        // Switch to enemy turn after damage
        turnState = TurnState::Enemy;
        enemyTurnPending = true;
        enemyTurnDelayClock.restart();
    }
    // Handle enemy turn: wait for health bar animation, then wait 1s, then show action label, then wait 1s, then apply damage
    static sf::Clock enemyPostAnimClock;
    static bool enemyPostAnimWaitStarted = false;
    if (enemyTurnPending) {
        // Wait until enemy health bar animation is finished
        bool healthBarDone = true;
        if (!enemies.empty() && displayedEnemyHealth.size() > 0) {
            healthBarDone = (std::abs(displayedEnemyHealth[0] - enemies[0].health) < 0.01f);
        }
        if (healthBarDone) {
            // Wait 1 second after animation before showing announcement
            if (!enemyPostAnimWaitStarted) {
                enemyPostAnimClock.restart();
                enemyPostAnimWaitStarted = true;
                return;
            }
            if (enemyPostAnimWaitStarted && enemyPostAnimClock.getElapsedTime().asSeconds() < 1.0f) {
                return;
            }
            // Now show action announcement and start damage delay
            if (!this->enemyMoveAnnounced && !enemies.empty()) {
                const Enemy& enemy = enemies[0];
                std::vector<Move> unlockedMoves;
                for (const auto& move : enemy.moves) {
                    if (enemy.level >= move.unlockLevel) unlockedMoves.push_back(move);
                }
                if (!unlockedMoves.empty()) {
                    this->pendingEnemyMoveIdx = rand() % unlockedMoves.size();
                    lastActionLabel = enemy.name + " uses " + unlockedMoves[this->pendingEnemyMoveIdx].name + "!";
                } else {
                    this->pendingEnemyMoveIdx = -1;
                    lastActionLabel = enemy.name + " attacks!";
                }
                this->enemyMoveAnnounced = true;
                enemyTurnDelayClock.restart();
            }
            if (this->enemyMoveAnnounced && enemyTurnDelayClock.getElapsedTime().asSeconds() >= enemyTurnDelaySeconds) {
                if (!enemies.empty()) {
                    const Enemy& enemy = enemies[0];
                    int enemyDmg = 0;
                    if (!enemy.moves.empty() && this->pendingEnemyMoveIdx >= 0) {
                        std::vector<Move> unlockedMoves;
                        for (const auto& move : enemy.moves) {
                            if (enemy.level >= move.unlockLevel) unlockedMoves.push_back(move);
                        }
                        if (this->pendingEnemyMoveIdx < (int)unlockedMoves.size()) {
                            enemyDmg = std::max(1, unlockedMoves[this->pendingEnemyMoveIdx].damage - player.defense);
                        }
                    }
                    if (enemyDmg == 0) {
                        enemyDmg = std::max(1, enemy.baseDamage - player.defense);
                    }
                    player.health -= enemyDmg;
                    if (player.health < 0) player.health = 0;
                }
                turnState = TurnState::Player;
                enemyTurnPending = false;
                this->enemyMoveAnnounced = false;
                this->pendingEnemyMoveIdx = -1;
                enemyPostAnimWaitStarted = false;
            }
        } else {
            // If health bar is still animating, reset post-anim wait
            enemyPostAnimWaitStarted = false;
        }
    }
    // Draw player health at bottom middle
    float healthBarWidth = 320.f;
    float healthBarHeight = 32.f;
    float winWidth = window.getSize().x;
    float winHeight = window.getSize().y;
    float barX = winWidth / 2.f - healthBarWidth / 2.f;
    float barY = winHeight - healthBarHeight - 32.f;
    int maxHealth = player.maxHealth > 0 ? player.maxHealth : 1;
    // Animate displayed health (time-based, not frame-based)
    float animSpeed = 30.0f; // units per second
    float dt = healthAnimClock.restart().asSeconds();
    if (displayedPlayerHealth != player.health) {
        float diff = player.health - displayedPlayerHealth;
        float step = animSpeed * dt;
        if (std::abs(diff) < step) displayedPlayerHealth = (float)player.health;
        else displayedPlayerHealth += (diff > 0 ? step : -step);
    }
    float healthPercent = displayedPlayerHealth / (float)maxHealth;
    if (healthPercent < 0.f) healthPercent = 0.f;
    if (healthPercent > 1.f) healthPercent = 1.f;
    sf::RectangleShape healthBg(sf::Vector2f(healthBarWidth, healthBarHeight));
    healthBg.setFillColor(sf::Color(30, 60, 30, 200));
    healthBg.setOutlineColor(sf::Color::White);
    healthBg.setOutlineThickness(2.f);
    healthBg.setPosition(sf::Vector2f(barX, barY));
    window.draw(healthBg);
    sf::RectangleShape healthFg(sf::Vector2f(healthBarWidth * healthPercent, healthBarHeight));
    healthFg.setFillColor(sf::Color(80, 220, 80));
    healthFg.setPosition(sf::Vector2f(barX, barY));
    window.draw(healthFg);
    // Draw 'Health' label to the left of the bar
    sf::Text healthLabel(font, "Health", 24);
    healthLabel.setFillColor(sf::Color::White);
    auto labelBounds = healthLabel.getLocalBounds();
    float labelX = barX - labelBounds.size.x - 16.f;
    float labelY = barY + healthBarHeight / 2.f - labelBounds.size.y / 2.f - 4.f;
    healthLabel.setPosition(sf::Vector2f(labelX, labelY));
    window.draw(healthLabel);
    // Draw health numbers inside the bar
    std::string healthText = std::to_string((int)displayedPlayerHealth) + "/" + std::to_string(maxHealth);
    sf::Text hpText(font, healthText, 24);
    hpText.setFillColor(sf::Color::White);
    auto bounds = hpText.getLocalBounds();
    hpText.setPosition(sf::Vector2f(barX + healthBarWidth / 2.f - bounds.size.x / 2.f, barY + healthBarHeight / 2.f - bounds.size.y / 2.f - 4.f));
    window.draw(hpText);
}

void BattleScreen::drawCharacters(sf::RenderWindow& window) {
    // Center player on left half, scale up for visibility
    if (playerIdleAnim) {
        float px = window.getSize().x * 0.25f;
        float py = window.getSize().y * 0.66f; // Lower third for feet alignment
        static sf::Clock animClock;
        float dt = animClock.restart().asSeconds();
        playerIdleAnim->setScale(sf::Vector2f(8.f, 8.f)); // Even larger for visibility
        playerIdleAnim->update(dt);
        playerIdleAnim->draw(window, sf::Vector2f(px, py));
    }
    // Center enemy on right half, scale up for visibility and mirror horizontally
    for (size_t i = 0; i < enemyIdleAnims.size(); ++i) {
        AnimatedSprite* anim = enemyIdleAnims[i];
        float ex = window.getSize().x * 0.75f;
        float ey = window.getSize().y * 0.66f; // Lower third for feet alignment
        if (anim) {
            static sf::Clock animClock;
            float dt = animClock.restart().asSeconds();
            anim->setScale(sf::Vector2f(-8.f, 8.f)); // Even larger and mirrored horizontally
            anim->update(dt);
            anim->draw(window, sf::Vector2f(ex, ey));
        } else {
            sf::CircleShape enemyShape(320.f); // much bigger enemy shape
            enemyShape.setFillColor(sf::Color(180, 80, 80));
            enemyShape.setPosition(sf::Vector2f(ex - 320.f, ey - 320.f));
            window.draw(enemyShape);
        }
        // Draw enemy health bar below enemy
        if (i < enemies.size()) {
            const Enemy& enemy = enemies[i];
            float barWidth = 320.f;
            float barHeight = 24.f;
            float barX = ex - barWidth / 2.f;
            float barY = ey + 100.f; // below feet
            int maxHealth = enemy.maxHealth > 0 ? enemy.maxHealth : 1;
            // Animate displayed health for each enemy (time-based)
            if (displayedEnemyHealth.size() != enemies.size()) displayedEnemyHealth.resize(enemies.size(), 0.f);
            float animSpeed = 30.0f; // units per second
            float dt = healthAnimClock.getElapsedTime().asSeconds();
            if (displayedEnemyHealth[i] != enemy.health) {
                float diff = enemy.health - displayedEnemyHealth[i];
                float step = animSpeed * dt;
                if (std::abs(diff) < step) displayedEnemyHealth[i] = (float)enemy.health;
                else displayedEnemyHealth[i] += (diff > 0 ? step : -step);
            }
            float healthPercent = displayedEnemyHealth[i] / (float)maxHealth;
            if (healthPercent < 0.f) healthPercent = 0.f;
            if (healthPercent > 1.f) healthPercent = 1.f;
            // Draw background
            sf::RectangleShape healthBg(sf::Vector2f(barWidth, barHeight));
            healthBg.setFillColor(sf::Color(60, 30, 30, 200));
            healthBg.setOutlineColor(sf::Color::White);
            healthBg.setOutlineThickness(2.f);
            healthBg.setPosition(sf::Vector2f(barX, barY));
            window.draw(healthBg);
            // Draw foreground
            sf::RectangleShape healthFg(sf::Vector2f(barWidth * healthPercent, barHeight));
            healthFg.setFillColor(sf::Color(220, 80, 80));
            healthFg.setPosition(sf::Vector2f(barX, barY));
            window.draw(healthFg);
            // Draw 'Health' label to the left of the bar
            sf::Text healthLabel(font, "Health", 18);
            healthLabel.setFillColor(sf::Color::White);
            auto labelBounds = healthLabel.getLocalBounds();
            float labelX = barX - labelBounds.size.x - 12.f;
            float labelY = barY + barHeight / 2.f - labelBounds.size.y / 2.f - 2.f;
            healthLabel.setPosition(sf::Vector2f(labelX, labelY));
            window.draw(healthLabel);
            // Draw health numbers inside the bar
            std::string healthText = std::to_string((int)displayedEnemyHealth[i]) + "/" + std::to_string(maxHealth);
            sf::Text hpText(font, healthText, 18);
            hpText.setFillColor(sf::Color::White);
            auto bounds = hpText.getLocalBounds();
            hpText.setPosition(sf::Vector2f(barX + barWidth / 2.f - bounds.size.x / 2.f, barY + barHeight / 2.f - bounds.size.y / 2.f - 2.f));
            window.draw(hpText);
        }
    }
}

void BattleScreen::drawIntro(sf::RenderWindow& window) {
    if (introState == IntroState::Done) return;
    // Set even larger font size
    introTextObj.setCharacterSize(108);
    // Calculate text bounds
    auto bounds = introTextObj.getLocalBounds();
    float textWidth = bounds.size.x;
    float textHeight = bounds.size.y;
    // Center box and text, double the padding
    float boxPaddingX = 96.f;
    float boxPaddingY = 64.f;
    float boxWidth = textWidth + boxPaddingX * 2;
    float boxHeight = textHeight + boxPaddingY * 2;
    float boxX = window.getSize().x / 2.f - boxWidth / 2.f;
    float boxY = window.getSize().y / 2.f - boxHeight / 2.f;
    // Draw shadow around the box
    float shadowOffset = 16.f;
    sf::RectangleShape shadow(sf::Vector2f(boxWidth + shadowOffset * 2, boxHeight + shadowOffset * 2));
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    shadow.setPosition(sf::Vector2f(boxX - shadowOffset, boxY - shadowOffset));
    window.draw(shadow);
    // Draw main box
    sf::RectangleShape bgBox(sf::Vector2f(boxWidth, boxHeight));
    bgBox.setFillColor(sf::Color(40, 40, 60, 220));
    bgBox.setOutlineColor(sf::Color::White);
    bgBox.setOutlineThickness(8.f);
    bgBox.setPosition(sf::Vector2f(boxX, boxY));
    window.draw(bgBox);
    // Center text in box
    introTextObj.setPosition(sf::Vector2f(boxX + boxPaddingX, boxY + boxPaddingY));
    window.draw(introTextObj);
}

void BattleScreen::drawOptions(sf::RenderWindow& window) {
    // Options box layout
    float boxWidth = 400.f, boxHeight = 320.f;
    float x = window.getSize().x - boxWidth - 48.f;
    float y = window.getSize().y - boxHeight - 48.f;
    sf::RectangleShape box(sf::Vector2f(boxWidth, boxHeight));
    box.setFillColor(sf::Color(40, 40, 60, 220));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(4.f);
    box.setPosition(sf::Vector2f(x, y));
    window.draw(box);

    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text opt(font, options[i], 40);
        // Highlight if selected by keyboard or hovered by mouse
        if (static_cast<int>(i) == selectedOption || static_cast<int>(i) == hoveredOption) {
            opt.setFillColor(sf::Color(180, 220, 255));
        } else {
            opt.setFillColor(sf::Color::White);
        }
        auto obounds = opt.getLocalBounds();
        float optWidth = obounds.size.x;
        float optHeight = obounds.size.y;
        opt.setPosition(sf::Vector2f(x + 32.f, y + 32.f + i * 64.f));
        window.draw(opt);
    }

    // If Fight is selected and clicked, show equipped moves (to the left of options box)
    if (selectedOption == 0 && showMoves) {
        auto equippedMoves = player.getEquippedMoves();
        float movesBoxWidth = boxWidth - 32.f;
        float movesBoxHeight = 180.f;
        float movesBoxX = x - movesBoxWidth - 32.f;
        float movesBoxY = y;
        sf::RectangleShape movesBox(sf::Vector2f(movesBoxWidth, movesBoxHeight));
        movesBox.setFillColor(sf::Color(30, 30, 50, 230));
        movesBox.setOutlineColor(sf::Color(180, 220, 255));
        movesBox.setOutlineThickness(2.f);
        movesBox.setPosition(sf::Vector2f(movesBoxX, movesBoxY));
        window.draw(movesBox);
        for (size_t i = 0; i < equippedMoves.size(); ++i) {
            sf::Text moveText(font, equippedMoves[i].name, 32);
            moveText.setFillColor(sf::Color::White);
            moveText.setPosition(sf::Vector2f(movesBoxX + 16.f, movesBoxY + 16.f + i * 40.f));
            window.draw(moveText);
        }
    }

    // If Items is selected and clicked, show consumables popup (to the left of options box)
    if (selectedOption == 1 && showItemsPopup) {
        float itemsBoxWidth = boxWidth - 32.f;
        float itemsBoxHeight = 180.f;
        float itemsBoxX = x - itemsBoxWidth - 32.f;
        float itemsBoxY = y;
        sf::RectangleShape itemsBox(sf::Vector2f(itemsBoxWidth, itemsBoxHeight));
        itemsBox.setFillColor(sf::Color(30, 50, 30, 230));
        itemsBox.setOutlineColor(sf::Color(180, 255, 180));
        itemsBox.setOutlineThickness(2.f);
        itemsBox.setPosition(sf::Vector2f(itemsBoxX, itemsBoxY));
        window.draw(itemsBox);
        for (size_t i = 0; i < player.inventory.size(); ++i) {
            sf::Text itemText(font, player.inventory[i], 32);
            itemText.setFillColor(sf::Color::White);
            itemText.setPosition(sf::Vector2f(itemsBoxX + 16.f, itemsBoxY + 16.f + i * 40.f));
            window.draw(itemText);
        }
    }

    // If Defend is selected, show a prepping message (visual only)
    if (selectedOption == 2 && defendActive) {
        sf::Text defendText(font, "Defending!", 36);
        defendText.setFillColor(sf::Color(220, 220, 100));
        defendText.setPosition(sf::Vector2f(x + boxWidth / 2.f - 80.f, y + boxHeight - 60.f));
        window.draw(defendText);
    }

    // If Run is selected and clicked, show result only if successful
    if (selectedOption == 3 && runAttempted && runSuccess) {
        sf::Text runText(font, "Escaped!", 36);
        runText.setFillColor(sf::Color(100, 220, 100));
        runText.setPosition(sf::Vector2f(x + boxWidth / 2.f - 120.f, y + boxHeight - 60.f));
        window.draw(runText);
    }
}

void BattleScreen::updateIntro(float dt) {
    if (introState == IntroState::Done) return;
    introTimer += dt;
    if (introState == IntroState::FadeIn) {
        float alpha = std::min(1.f, introTimer / fadeInDuration);
        introTextObj.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha * 255)));
        if (introTimer >= fadeInDuration) {
            introState = IntroState::Display;
            introTimer = 0.f;
        }
    } else if (introState == IntroState::Display) {
        introTextObj.setFillColor(sf::Color(255, 255, 255, 255));
        if (introTimer >= displayDuration) {
            introState = IntroState::FadeOut;
            introTimer = 0.f;
        }
    } else if (introState == IntroState::FadeOut) {
        float alpha = 1.f - (introTimer / fadeOutDuration);
        if (alpha < 0.f) alpha = 0.f;
        if (alpha > 1.f) alpha = 1.f;
        introTextObj.setFillColor(sf::Color(255, 255, 255, static_cast<uint8_t>(alpha * 255)));
        if (introTimer >= fadeOutDuration) {
            introTextObj.setFillColor(sf::Color(255, 255, 255, 0));
            introState = IntroState::Done;
            introTimer = 0.f;
        }
    }
}

void BattleScreen::setIntroText(const std::string& text) {
    introText = text;
    introTextObj.setString(text);
    introTextObj.setFillColor(sf::Color(255, 255, 255, 0));
    introState = IntroState::FadeIn;
    introTimer = 0.f;
}


bool BattleScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    // Mouse hover effect for options
    if (const auto* mouseMoveEvent = event.getIf<sf::Event::MouseMoved>()) {
        float boxWidth = 400.f, boxHeight = 320.f;
        float x = window.getSize().x - boxWidth - 48.f;
        float y = window.getSize().y - boxHeight - 48.f;
        sf::Vector2f mousePos(static_cast<float>(mouseMoveEvent->position.x), static_cast<float>(mouseMoveEvent->position.y));
        hoveredOption = -1;
        for (size_t i = 0; i < options.size(); ++i) {
            float optX = x + 32.f;
            float optY = y + 32.f + i * 64.f;
            float optW = 336.f;
            float optH = 48.f;
            sf::FloatRect optRect(sf::Vector2f(optX, optY), sf::Vector2f(optW, optH));
            if (optRect.contains(mousePos)) {
                hoveredOption = static_cast<int>(i);
                break;
            }
        }
        return false;
    }
    // Only allow input on player turn
    if (turnState == TurnState::Player) {
        if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            auto key = keyEvent->code;
            if (key == sf::Keyboard::Key::Down) {
                selectedOption = (selectedOption + 1) % options.size();
                hoveredOption = selectedOption;
            } else if (key == sf::Keyboard::Key::Up) {
                selectedOption = (selectedOption - 1 + options.size()) % options.size();
                hoveredOption = selectedOption;
            } else if (key == sf::Keyboard::Key::Enter || key == sf::Keyboard::Key::Space) {
                // Activate selected option
                if (selectedOption == 0) { // Fight
                    showMoves = !showMoves;
                    showItemsPopup = false;
                    defendActive = false;
                    runAttempted = false;
                } else if (selectedOption == 1) { // Items
                    showItemsPopup = !showItemsPopup;
                    showMoves = false;
                    defendActive = false;
                    runAttempted = false;
                    lastActionLabel = player.name + " uses an item!";
                } else if (selectedOption == 2) { // Defend
                    defendActive = true;
                    showMoves = false;
                    showItemsPopup = false;
                    runAttempted = false;
                    lastActionLabel = player.name + " defends!";
                } else if (selectedOption == 3) { // Run
                    runAttempted = true;
                    showMoves = false;
                    showItemsPopup = false;
                    defendActive = false;
                    runSuccess = (rand() % 2 == 0);
                    lastActionLabel = player.name + (runSuccess ? " escapes!" : " fails to escape!");
                    if (runSuccess) {
                        battleOver = true;
                    } else {
                        turnState = TurnState::Enemy;
                        enemyTurnPending = true;
                        enemyMoveAnnounced = false;
                        pendingEnemyMoveIdx = -1;
                    }
                }
            } else if (key == sf::Keyboard::Key::Escape) {
                // Close popups
                showMoves = false;
                showItemsPopup = false;
                defendActive = false;
                runAttempted = false;
            }
            return true;
        }
    }

    // Only allow mouse input on player turn
    if (turnState == TurnState::Player) {
        if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                // Calculate option box position and size
                float boxWidth = 400.f, boxHeight = 320.f;
                float x = window.getSize().x - boxWidth - 48.f;
                float y = window.getSize().y - boxHeight - 48.f;
                sf::Vector2f mousePos(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));
                // Check which option was clicked
                for (size_t i = 0; i < options.size(); ++i) {
                    float optX = x + 32.f;
                    float optY = y + 32.f + i * 64.f;
                    float optW = 336.f; // Approximate width for click area
                    float optH = 48.f;  // Height for click area
                    sf::FloatRect optRect(sf::Vector2f(optX, optY), sf::Vector2f(optW, optH));
                    if (optRect.contains(mousePos)) {
                        selectedOption = static_cast<int>(i);
                        // Activate selected option
                    if (selectedOption == 0) { // Fight
                        showMoves = !showMoves;
                        showItemsPopup = false;
                        defendActive = false;
                        runAttempted = false;
                    } else if (selectedOption == 1) { // Items
                        showItemsPopup = !showItemsPopup;
                        showMoves = false;
                        defendActive = false;
                        runAttempted = false;
                    } else if (selectedOption == 2) { // Defend
                        defendActive = true;
                        showMoves = false;
                        showItemsPopup = false;
                        runAttempted = false;
                    } else if (selectedOption == 3) { // Run
                        runAttempted = true;
                        showMoves = false;
                        showItemsPopup = false;
                        defendActive = false;
                        runSuccess = (rand() % 2 == 0);
                        lastActionLabel = player.name + (runSuccess ? " escapes!" : " fails to escape!");
                        if (runSuccess) {
                            battleOver = true;
                        } else {
                            turnState = TurnState::Enemy;
                            enemyTurnPending = true;
                            this->enemyMoveAnnounced = false;
                            this->pendingEnemyMoveIdx = -1;
                        }
                    }
                        return true;
                    }
                }
                // If moves popup is open, check for move selection
                if (showMoves && selectedOption == 0) {
                    auto equippedMoves = player.getEquippedMoves();
                    float movesBoxWidth = 400.f - 32.f;
                    float movesBoxHeight = 180.f;
                    float movesBoxX = window.getSize().x - 400.f - 48.f - movesBoxWidth - 32.f;
                    float movesBoxY = window.getSize().y - 320.f - 48.f;
                    for (size_t i = 0; i < equippedMoves.size(); ++i) {
                        float moveX = movesBoxX + 16.f;
                        float moveY = movesBoxY + 16.f + i * 40.f;
                        float moveW = movesBoxWidth - 32.f;
                        float moveH = 32.f;
                        sf::FloatRect moveRect(sf::Vector2f(moveX, moveY), sf::Vector2f(moveW, moveH));
                        if (moveRect.contains(sf::Vector2f(mouseEvent->position.x, mouseEvent->position.y))) {
                            // Show label immediately, delay damage
                            if (!enemies.empty()) {
                                lastActionLabel = player.name + " uses " + equippedMoves[i].name + "!";
                                playerMovePending = true;
                                pendingPlayerMoveIdx = (int)i;
                                playerMoveDelayClock.restart();
                            }
                            showMoves = false; // Close popup after attack
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool BattleScreen::isComplete() const {
    return battleOver;
}
