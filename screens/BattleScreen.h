#pragma once
#include "../core/Enemy.h"
#include "../core/Character.h"
#include <SFML/Graphics.hpp>
#include "ScreenBase.h"
#include <string>
#include "../AnimatedSprite.h"
#include <vector>

class BattleScreen : public ScreenBase {
    enum class TurnState { Player, Enemy } turnState = TurnState::Player;
    // For time-based health animation
    sf::Clock healthAnimClock;
    // Animated health bar values
    float displayedPlayerHealth = 0.f;
    std::vector<float> displayedEnemyHealth;
    std::string lastActionLabel; // Declare lastActionLabel as a member variable
    // Enemy turn state variables
    bool enemyMoveAnnounced = false;
    int pendingEnemyMoveIdx = -1;
public:
    BattleScreen(const sf::Font& font);
    void start(int floorNum, const Character& player, AnimatedSprite* playerIdleAnimPtr, const std::vector<Character>& enemies, const std::vector<AnimatedSprite*>& enemyIdleAnims);
    void start(int floorNum, const Character& player, AnimatedSprite* playerIdleAnimPtr, const std::vector<Enemy>& enemies, const std::vector<AnimatedSprite*>& enemyIdleAnims);
    void start(int floorNum) override;
    void draw(sf::RenderWindow& window) override;
    bool handleEvent(const sf::Event& event, sf::RenderWindow& window) override;
    bool isComplete() const override;
    void updateIntro(float dt);
protected:
    const sf::Font& font;
    int floorNumber = 1;
    Character player;
    std::vector<Enemy> enemies;
    AnimatedSprite* playerIdleAnim = nullptr;
    std::vector<AnimatedSprite*> enemyIdleAnims;
    bool battleOver = false;
    // Intro text logic
    sf::Text introTextObj;
    std::string introText;
    float introTimer = 0.f;
    float introAlpha = 0.f;
    enum class IntroState { FadeIn, Display, FadeOut, Done } introState = IntroState::Done;
    float fadeInDuration = 1.0f;
    float displayDuration = 2.0f;
    float fadeOutDuration = 1.0f;
    void drawIntro(sf::RenderWindow& window);
    void setIntroText(const std::string& text);
    int selectedOption = 0;
    std::vector<std::string> options = {"Fight", "Items", "Defend", "Run"};
    // Interactive options box state
    bool showMoves = false;
    bool showItemsPopup = false;
    bool defendActive = false;
    bool runAttempted = false;
    bool runSuccess = false;
    int hoveredOption = -1;
    void drawOptions(sf::RenderWindow& window);
    void drawCharacters(sf::RenderWindow& window);

    // Timer and state for enemy turn delay
    sf::Clock enemyTurnDelayClock;
    bool enemyTurnPending = false;
    float enemyTurnDelaySeconds = 1.0f;
    // Timer and state for player move damage delay
    sf::Clock playerMoveDelayClock;
    bool playerMovePending = false;
    int pendingPlayerMoveIdx = -1;
    float playerMoveDelaySeconds = 1.0f;
};
