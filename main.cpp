#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <variant>
#include <filesystem>
#include "screens/MenuScreen.h"
#include "screens/NameEntryScreen.h"
#include "screens/SpriteSelectScreen.h"
#include "screens/TransitionScreen.h"
#include "screens/DoorSelectionScreen.h"
#include "screens/PopupMenu.h"
#include "screens/CharacterStatsScreen.h"
#include "core/Character.h"
#include "core/Enemy.h"
#include "screens/CommonEnemyScreen.h"
#include "screens/MysteryScreen.h"
#include "screens/MerchantScreen.h"
#include "screens/RestSiteScreen.h"
#include "screens/EliteScreen.h"
#include "screens/TreasureScreen.h"
#include "screens/MiniBossScreen.h"
#include "screens/MainBossScreen.h"
#include "AnimatedSprite.h"
#include "screens/BattleScreen.h"

enum class GameScreen { Menu, NameEntry, SpriteSelect, Transition, Event, Room };

int main() {
    srand(static_cast<unsigned int>(time(0)));
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Ironbound: Turn-Based Battle Game", sf::Style::None);
    window.setPosition(sf::Vector2i(0, 0));
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        // Fallback: exit if font not found
        return 1;
    }
    int hovered = -1;
    GameScreen currentScreen = GameScreen::Menu;
    std::string playerName;
    MenuScreen menuScreen(font);
    NameEntryScreen nameEntryScreen(font);
    SpriteSelectScreen spriteSelectScreen(font);
    TransitionScreen transitionScreen(font);
    DoorSelectionScreen doorSelectionScreen(font);
    doorSelectionScreen.setWindowSize(window.getSize());
    PopupMenu popupMenu(font);
    CharacterStatsScreen statsScreen(font);
    CommonEnemyScreen commonEnemy(font);
    MysteryScreen mysteryScreen(font);
    MerchantScreen merchantScreen(font);
    RestSiteScreen restSiteScreen(font);
    EliteScreen eliteScreen(font);
    TreasureScreen treasureScreen(font);
    MiniBossScreen miniBossScreen(font);
    MainBossScreen mainBossScreen(font);
    BattleScreen battleScreen(font);
    // Track the active room screen
    ScreenBase* activeRoomScreen = nullptr;
    sf::RectangleShape menuButton(sf::Vector2f(120.f, 48.f));
    menuButton.setFillColor(sf::Color(60, 60, 100));
    menuButton.setOutlineColor(sf::Color::White);
    menuButton.setOutlineThickness(2.f);
    menuButton.setPosition(sf::Vector2f(window.getSize().x - 136.f, 16.f));
    sf::Text menuText(font, "Menu", 28);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition(sf::Vector2f(menuButton.getPosition().x + 24.f, menuButton.getPosition().y + 8.f));
    // Character button (hidden until first door appears)
    sf::RectangleShape charButton(sf::Vector2f(180.f, 48.f));
    charButton.setFillColor(sf::Color(60, 100, 60));
    charButton.setOutlineColor(sf::Color::White);
    charButton.setOutlineThickness(2.f);
    charButton.setPosition(sf::Vector2f(window.getSize().x - 332.f, 16.f));
    sf::Text charText(font, "Character", 28);
    charText.setFillColor(sf::Color::White);
    charText.setPosition(sf::Vector2f(charButton.getPosition().x + 24.f, charButton.getPosition().y + 8.f));
    bool menuHovered = false;
    bool charHovered = false;
    bool showCharButton = false;
    int spriteHovered = -1;
    int spriteSelected = -1;
    int doorSelected = -1;
    int currentFloor = 1;
    Character playerCharacter("", "", 100, 100, 10, 10, 0, 1); // Default, will be set after selection
    bool hasPlayerCharacter = false;
    sf::Clock clock;
    float fadeAlpha = 0.f;
    bool fading = false;
    bool fadeOut = false;
    GameScreen nextScreen = currentScreen;
    int popupSelected = -1;

    while (window.isOpen()) {
        float dt = (popupMenu.isVisible() || statsScreen.isVisible()) ? 0.f : clock.restart().asSeconds();
        int menuSelected = -1;
        // Only reset hover and run game logic if no modal is visible
        if (!popupMenu.isVisible() && !statsScreen.isVisible() && currentScreen == GameScreen::SpriteSelect) spriteHovered = -1;
        menuHovered = false;
        charHovered = false;
        // Poll events and handle modal overlays first
        while (auto eventOpt = window.pollEvent()) {
            const auto& event = *eventOpt;
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            // Always handle popup menu and stats screen overlays first
            if (popupMenu.isVisible()) {
                popupMenu.handleEvent(event, window, popupSelected);
                continue;
            }
            if (statsScreen.isVisible()) {
                statsScreen.handleEvent(event, window);
                continue;
            }
            // Only allow stats screen in Event, Battle, Log screens
            // Allow stats screen in all screens except Menu, NameEntry, SpriteSelect, and Transition
            bool canShowStats = (
                currentScreen != GameScreen::Menu &&
                currentScreen != GameScreen::NameEntry &&
                currentScreen != GameScreen::SpriteSelect &&
                currentScreen != GameScreen::Transition
            );
            if (!popupMenu.isVisible() && !statsScreen.isVisible() && !fading && canShowStats) {
                if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
                    if (keyEvent->code == sf::Keyboard::Key::I && hasPlayerCharacter) {
                        std::vector<std::string> moves;
                        for (const auto& m : playerCharacter.getUnlockedMoves()) moves.push_back(m.name);
                        std::vector<std::string> inventory = playerCharacter.inventory;
                        std::vector<std::string> equipped = playerCharacter.equipped;
                        AnimatedSprite* idleAnim = nullptr;
                        if (spriteSelected >= 0 && spriteSelected < spriteSelectScreen.characterAnimations.size()) {
                            auto& animMap = spriteSelectScreen.characterAnimations[spriteSelected];
                            auto it = animMap.find("Idle");
                            if (it != animMap.end()) idleAnim = &it->second;
                        }
                        statsScreen.setStats(
                            playerCharacter.name,
                            playerCharacter.charClass,
                            playerCharacter.health,
                            playerCharacter.health,
                            playerCharacter.level,
                            playerCharacter.xp,
                            playerCharacter.level * 30,
                            playerCharacter.baseDamage,
                            playerCharacter.defense,
                            playerCharacter.speed,
                            moves,
                            idleAnim,
                            inventory,
                            equipped
                        );
                        statsScreen.show();
                        continue;
                    }
                    if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        popupMenu.show();
                        continue;
                    }
                }
                if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    if (menuButton.getGlobalBounds().contains(worldPos)) {
                        popupMenu.show();
                        continue;
                    }
                    // Open stats screen if character button is clicked
                    if (showCharButton && charButton.getGlobalBounds().contains(worldPos) && hasPlayerCharacter) {
                        std::vector<std::string> moves;
                        for (const auto& m : playerCharacter.getUnlockedMoves()) moves.push_back(m.name);
                        std::vector<std::string> inventory = playerCharacter.inventory;
                        std::vector<std::string> equipped = playerCharacter.equipped;
                        AnimatedSprite* idleAnim = nullptr;
                        if (spriteSelected >= 0 && spriteSelected < spriteSelectScreen.characterAnimations.size()) {
                            auto& animMap = spriteSelectScreen.characterAnimations[spriteSelected];
                            auto it = animMap.find("Idle");
                            if (it != animMap.end()) idleAnim = &it->second;
                        }
                        statsScreen.setStats(
                            playerCharacter.name,
                            playerCharacter.charClass,
                            playerCharacter.health,
                            playerCharacter.health,
                            playerCharacter.level,
                            playerCharacter.xp,
                            playerCharacter.level * 30,
                            playerCharacter.baseDamage,
                            playerCharacter.defense,
                            playerCharacter.speed,
                            moves,
                            idleAnim,
                            inventory,
                            equipped
                        );
                        statsScreen.show();
                        continue;
                    }
                }
            }
            // Hover effects for buttons
            if (currentScreen != GameScreen::Menu) {
                if (event.is<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    if (menuButton.getGlobalBounds().contains(worldPos)) menuHovered = true;
                    if (showCharButton && charButton.getGlobalBounds().contains(worldPos)) charHovered = true;
                }
            }
        if (!fading && !popupMenu.isVisible() && !statsScreen.isVisible()) {
            if (currentScreen == GameScreen::Menu) {
                menuScreen.handleEvent(event, window, hovered, menuSelected);
                if (menuSelected == 0) { fadeOut = true; fading = true; nextScreen = GameScreen::NameEntry; }
                else if (menuSelected == 1) { fadeOut = true; fading = true; nextScreen = GameScreen::Room; }
                else if (menuSelected == 2) window.close();
            } else if (currentScreen == GameScreen::NameEntry) {
                if (nameEntryScreen.handleEvent(event, playerName)) {
                    fadeOut = true; fading = true; nextScreen = GameScreen::SpriteSelect;
                }
            } else if (currentScreen == GameScreen::SpriteSelect) {
                if (spriteSelectScreen.handleEvent(event, window, spriteHovered, spriteSelected)) {
                static std::vector<Character> allChars = Character::loadCharactersFromJSON("characters.json");
                std::string selectedName = (spriteSelected >= 0 && spriteSelected < spriteSelectScreen.characterNames.size()) ? spriteSelectScreen.characterNames[spriteSelected] : "";
                auto toLowerTrim = [](const std::string& s) {
                    std::string out;
                    for (char ch : s) if (!isspace(ch)) out += std::tolower(ch);
                    return out;
                };
                std::string selectedNameNorm = toLowerTrim(selectedName);
                Character* selectedChar = nullptr;
                for (auto& c : allChars) {
                    if (toLowerTrim(c.name) == selectedNameNorm) {
                        selectedChar = &c;
                        break;
                    }
                }
                if (!selectedChar) {
                    std::string message = "Error: Could not find character stats for '" + selectedName + "'.\nPlease check that the character name in the selection matches the name in characters.json.";
                    transitionScreen.setMessage(message);
                    fadeOut = true; fading = true; nextScreen = GameScreen::Transition;
                } else {
                    playerCharacter = *selectedChar;
                    hasPlayerCharacter = true;
                    std::string message = playerName + ", you have picked to be the " + selectedChar->name + ".\nGood luck... Many adventures await...\n\nPress any key or click to continue.";
                    transitionScreen.setMessage(message);
                    fadeOut = true; fading = true; nextScreen = GameScreen::Transition;
                }
                }
            } else if (currentScreen == GameScreen::Transition) {
                if (event.is<sf::Event::KeyPressed>() || event.is<sf::Event::MouseButtonPressed>()) {
                    doorSelectionScreen.start(currentFloor);
                    showCharButton = true;
                    fadeOut = true; fading = true; nextScreen = GameScreen::Event;
                }
            } else if (currentScreen == GameScreen::Event) {
                if (doorSelectionScreen.handleEvent(event, window, doorSelected)) {
                    std::vector<std::string> labels = doorSelectionScreen.getDoorLabels();
                    std::string selectedLabel = (doorSelected >= 0 && doorSelected < (int)labels.size()) ? labels[doorSelected] : "";
                    if (selectedLabel == "Common Enemy") {
                        // Select a random common enemy using CommonEnemy utility
                        Enemy selectedEnemy = CommonEnemyScreen::getRandomCommonEnemy("enemies.json");
                        // Load enemy idle animation (Flight.png for flying enemies, else Idle.png)
                        std::string flightPath = selectedEnemy.spriteFolder + "Flight.png";
                        std::string idlePath = selectedEnemy.spriteFolder + "Idle.png";
                        AnimatedSprite* enemyIdleAnim = nullptr;
                        bool hasIdle = std::filesystem::exists(idlePath);
                        bool hasFlight = std::filesystem::exists(flightPath);
                        if (hasIdle) {
                            enemyIdleAnim = new AnimatedSprite({idlePath}, 0.18f);
                            // Mirror horizontally for right side (SFML 3.x)
                            enemyIdleAnim->setScale(sf::Vector2f(-2.f, 2.f));
                        } else if (hasFlight) {
                            enemyIdleAnim = new AnimatedSprite({flightPath}, 0.18f);
                            // Mirror horizontally for right side (SFML 3.x)
                            enemyIdleAnim->setScale(sf::Vector2f(-2.f, 2.f));
                        }
                        // Load player idle animation
                        AnimatedSprite* idleAnim = nullptr;
                        if (spriteSelected >= 0 && spriteSelected < spriteSelectScreen.characterAnimations.size()) {
                            auto& animMap = spriteSelectScreen.characterAnimations[spriteSelected];
                            auto it = animMap.find("Idle");
                            if (it != animMap.end()) idleAnim = &it->second;
                        }
                        // Start battle with one enemy
                        battleScreen.start(currentFloor, playerCharacter, idleAnim, {selectedEnemy}, {enemyIdleAnim});
                        activeRoomScreen = &battleScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Mystery") {
                        mysteryScreen.start(currentFloor);
                        activeRoomScreen = &mysteryScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Merchant") {
                        merchantScreen.start(currentFloor);
                        activeRoomScreen = &merchantScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Rest Site") {
                        restSiteScreen.start(currentFloor);
                        activeRoomScreen = &restSiteScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Elite") {
                        eliteScreen.start(currentFloor);
                        activeRoomScreen = &eliteScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Treasure") {
                        treasureScreen.start(currentFloor);
                        activeRoomScreen = &treasureScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Mini-Boss") {
                        miniBossScreen.start(currentFloor);
                        activeRoomScreen = &miniBossScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else if (selectedLabel == "Main Boss") {
                        mainBossScreen.start(currentFloor);
                        activeRoomScreen = &mainBossScreen;
                        fadeOut = true; fading = true; nextScreen = GameScreen::Room;
                    } else {
                        // For now, just go to next floor for other doors
                        currentFloor++;
                        doorSelectionScreen.start(currentFloor);
                        fadeOut = true; fading = true; nextScreen = GameScreen::Event;
                    }
                }
            } else if (currentScreen == GameScreen::Room) {
                // Only interact with the active room screen
                if (activeRoomScreen) {
                    if (activeRoomScreen->handleEvent(event, window) && activeRoomScreen->isComplete()) {
                        currentFloor++;
                        doorSelectionScreen.start(currentFloor);
                        fadeOut = true; fading = true; nextScreen = GameScreen::Event;
                        activeRoomScreen = nullptr;
                    }
                }
            }
            }
        }
        window.clear(sf::Color(30, 30, 40));
        if (popupMenu.isVisible()) {
            // Only draw faded background and popup menu when menu is open
            sf::RectangleShape fadeBg(sf::Vector2f(window.getSize().x, window.getSize().y));
            fadeBg.setFillColor(sf::Color(0, 0, 0, 120));
            window.draw(fadeBg);
            popupMenu.draw(window);
            window.display();
            continue;
        }
        if (statsScreen.isVisible()) {
            sf::RectangleShape fadeBg(sf::Vector2f(window.getSize().x, window.getSize().y));
            fadeBg.setFillColor(sf::Color(0, 0, 0, 120));
            window.draw(fadeBg);
            statsScreen.draw(window);
            window.display();
            continue;
        }
        if (currentScreen == GameScreen::Menu) {
            menuScreen.draw(window, hovered);
        } else if (currentScreen == GameScreen::NameEntry) {
            nameEntryScreen.draw(window);
        } else if (currentScreen == GameScreen::SpriteSelect) {
            spriteSelectScreen.draw(window, spriteHovered, dt);
        } else if (currentScreen == GameScreen::Transition) {
            transitionScreen.draw(window);
        } else if (currentScreen == GameScreen::Event) {
            doorSelectionScreen.draw(window);
        } else if (currentScreen == GameScreen::Room) {
            // Draw only the active room screen
            if (activeRoomScreen) {
                // If the active room is a BattleScreen, update intro animation
                if (auto* battle = dynamic_cast<BattleScreen*>(activeRoomScreen)) {
                    battle->updateIntro(dt);
                }
                activeRoomScreen->draw(window);
            }
        }
        // Fade transition overlay
        if (fading || fadeAlpha > 0.f) {
            if (fading) {
                if (fadeOut) {
                    fadeAlpha += dt * 1.5f; // Fade out speed
                    if (fadeAlpha >= 1.f) {
                        fadeAlpha = 1.f;
                        currentScreen = nextScreen;
                        fadeOut = false;
                        fading = false;
                    }
                } else {
                    fadeAlpha -= dt * 1.5f; // Fade in speed
                    if (fadeAlpha <= 0.f) {
                        fadeAlpha = 0.f;
                        fading = false;
                    }
                }
            } else {
                fadeAlpha -= dt * 1.5f;
                if (fadeAlpha < 0.f) fadeAlpha = 0.f;
            }
            sf::RectangleShape fadeRect(sf::Vector2f(window.getSize().x, window.getSize().y));
            fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(fadeAlpha * 255)));
            window.draw(fadeRect);
        }
        if (currentScreen != GameScreen::Menu) {
            // Menu button hover effect
            if (menuHovered) menuButton.setFillColor(sf::Color(90, 90, 160));
            else menuButton.setFillColor(sf::Color(60, 60, 100));
            window.draw(menuButton);
            window.draw(menuText);
            // Character button hover effect
            if (showCharButton) {
                if (charHovered) charButton.setFillColor(sf::Color(90, 160, 90));
                else charButton.setFillColor(sf::Color(60, 100, 60));
                window.draw(charButton);
                window.draw(charText);
            }
        }
        window.display();
    }
    return 0;
}
