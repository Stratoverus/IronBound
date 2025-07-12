#include "Event.h"
#include <iostream>
#include <cstdlib>
#include <limits>

const std::vector<ChoiceEvent> Event::CHOICE_EVENTS = {
    {"You find a mysterious altar glowing with energy. What do you do?",
        {
            {"Touch the altar (Take 10 damage, gain 25 XP)", [](Character& p){ p.takeDamage(10); p.gainXP(25); /* ...existing code... */ }},
            {"Pray at the altar (Heal 15 HP, lose 10 XP)", [](Character& p){ p.health += 15; p.gainXP(-10); /* ...existing code... */ }},
            {"Ignore it and move on", [](Character&){ /* ...existing code... */ }}
        }
    },
    {"A wounded traveler offers you a potion in exchange for some XP. What do you do?",
        {
            {"Trade 15 XP for a potion (Heal 30 HP)", [](Character& p){ p.gainXP(-15); p.health += 30; /* ...existing code... */ }},
            {"Refuse the offer", [](Character&){ /* ...existing code... */ }}
        }
    },
    {"You find a cursed book. It promises power at a price.",
        {
            {"Read the book (Take 20 damage, gain 40 XP)", [](Character& p){ p.takeDamage(20); p.gainXP(40); /* ...existing code... */ }},
            {"Burn the book (Nothing happens)", [](Character&){ /* ...existing code... */ }},
            {"Leave it alone", [](Character&){ /* ...existing code... */ }}
        }
    },
    {"A healing spring bubbles up from the ground.",
        {
            {"Drink from the spring (Heal 20 HP, lose 10 XP)", [](Character& p){ p.health += 20; p.gainXP(-10); /* ...existing code... */ }},
            {"Rest by the spring (Heal 10 HP)", [](Character& p){ p.health += 10; /* ...existing code... */ }},
            {"Move on", [](Character&){ /* ...existing code... */ }}
        }
    },
    {"[PROCEDURAL] You encounter something unusual...",
        {
            {"[Procedural Option 1]", [](Character& p){ /* Placeholder, see below */ }},
            {"[Procedural Option 2]", [](Character& p){ /* Placeholder, see below */ }},
            {"[Procedural Option 3]", [](Character& p){ /* Placeholder, see below */ }}
        }
    }
};

void Event::trigger(Character& player, int floor) {
    int eventType = rand() % 4; // Now 4 types: battle, treasure, trap, choice
    switch (eventType) {
        case 0: // Procedural enemy
        {
            int base = 30 + floor * 5;
            int atk = 8 + floor * 2;
            int def = 2 + floor / 5;
            Character enemy("Floor " + std::to_string(floor) + " Foe", "Foe", base, atk, def, 0, 0, floor/10+1);
            // ...existing code...
            while (player.health > 0 && enemy.health > 0) {
                // ...existing code...
                // ...existing code...
                auto unlocked = player.getUnlockedMoves();
                // ...existing code...
                for (size_t i = 0; i < unlocked.size(); ++i) {
                    // ...existing code...
                }
                int action;
                std::cin >> action;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                int dmg = player.attackEnemy(enemy, action-1);
                // ...existing code...
                if (enemy.health > 0) {
                    int enemyAtk = rand() % enemy.getUnlockedMoves().size();
                    int edmg = enemy.attackEnemy(player, enemyAtk);
                    // ...existing code...
                }
            }
            if (player.health > 0) {
                // ...existing code...
                player.gainXP(10 + floor * 2);
                // ...existing code...
            } else {
                // ...existing code...
            }
        }
        break;
        case 1:
            std::cout << "You found a treasure chest! You gain " << (5 + floor) << " XP.\n";
            player.gainXP(5 + floor);
            break;
        case 2:
            std::cout << "A trap! You lose " << (10 + floor) << " HP.\n";
            player.takeDamage(10 + floor);
            break;
        case 3: // Choice event
        {
            int idx = rand() % CHOICE_EVENTS.size();
            // If procedural event, generate random scenario
            if (idx == (int)CHOICE_EVENTS.size() - 1) {
                // Procedural event
                static const char* objects[] = {"statue", "mirror", "fountain", "crystal", "ancient door"};
                static const char* actions[] = {"touch", "drink from", "study", "open", "break"};
                static const char* effects[] = {"power", "pain", "healing", "mystery", "curse"};
                int objIdx = rand() % 5;
                int actIdx = rand() % 5;
                int effIdx = rand() % 5;
                int hpChange = (rand() % 21) - 10; // -10 to +10
                int xpChange = (rand() % 31) - 10; // -10 to +20
                std::cout << "You find a " << objects[objIdx] << ". Do you " << actions[actIdx] << " it? It radiates " << effects[effIdx] << ".\n";
                std::cout << "1. " << actions[actIdx] << " the " << objects[objIdx] << " (" << (hpChange >= 0 ? "+" : "") << hpChange << " HP, " << (xpChange >= 0 ? "+" : "") << xpChange << " XP)\n";
                std::cout << "2. Ignore it and move on\n";
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (choice == 1) {
                    if (hpChange != 0) player.health += hpChange;
                    if (xpChange != 0) player.gainXP(xpChange);
                    std::cout << "You feel different! " << (hpChange ? (std::string((hpChange > 0) ? "+" : "") + std::to_string(hpChange) + " HP, ") : "") << (xpChange ? (std::string((xpChange > 0) ? "+" : "") + std::to_string(xpChange) + " XP") : "") << "\n";
                } else {
                    std::cout << "You leave it alone.\n";
                }
            } else {
                const ChoiceEvent& ce = CHOICE_EVENTS[idx];
                std::cout << ce.flavorText << "\n";
                for (size_t i = 0; i < ce.options.size(); ++i) {
                    std::cout << (i+1) << ". " << ce.options[i].description << "\n";
                }
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (choice >= 1 && choice <= (int)ce.options.size()) {
                    ce.options[choice-1].effect(player);
                } else {
                    std::cout << "You hesitate and nothing happens.\n";
                }
            }
        }
        break;
    }
}

void Event::miniBoss(Character& player, int floor) {
    int base = 80 + floor * 8;
    int atk = 18 + floor * 3;
    int def = 6 + floor / 3;
    Character miniboss("Mini-Boss Floor " + std::to_string(floor), "MiniBoss", base, atk, def, 0, 0, floor/10+2);
    std::cout << "A Mini-Boss blocks your path!\n";
    while (player.health > 0 && miniboss.health > 0) {
        std::cout << player.name << " HP: " << player.health << " | XP: " << player.xp << " | Level: " << player.level << std::endl;
        std::cout << miniboss.name << " HP: " << miniboss.health << std::endl;
        auto unlocked = player.getUnlockedMoves();
        std::cout << "Choose your attack:\n";
        for (size_t i = 0; i < unlocked.size(); ++i) {
            std::cout << (i+1) << ". " << unlocked[i].name << " (Damage: " << unlocked[i].damage << ")\n";
        }
        int action;
        std::cin >> action;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int dmg = player.attackEnemy(miniboss, action-1);
        std::cout << "You use " << unlocked[action-1].name << " and deal " << dmg << " damage!\n";
        if (miniboss.health > 0) {
            int enemyAtk = rand() % miniboss.getUnlockedMoves().size();
            int edmg = miniboss.attackEnemy(player, enemyAtk);
            std::cout << miniboss.name << " attacks you for " << edmg << " damage!\n";
        }
    }
    if (player.health > 0) {
        std::cout << "You defeated the " << miniboss.name << "!\n";
        player.gainXP(40 + floor * 3);
        std::cout << "You gained " << (40 + floor * 3) << " XP! Total XP: " << player.xp << std::endl;
    } else {
        std::cout << "You were defeated by the " << miniboss.name << "...\n";
    }
}

void Event::megaBoss(Character& player) {
    Character megaboss("MEGA BOSS: The Ironbound Overlord", "Overlord", 500, 50, 20, 0, 0, 10);
    std::cout << "The MEGA BOSS awaits!\n";
    while (player.health > 0 && megaboss.health > 0) {
        std::cout << player.name << " HP: " << player.health << " | XP: " << player.xp << " | Level: " << player.level << std::endl;
        std::cout << megaboss.name << " HP: " << megaboss.health << std::endl;
        auto unlocked = player.getUnlockedMoves();
        std::cout << "Choose your attack:\n";
        for (size_t i = 0; i < unlocked.size(); ++i) {
            std::cout << (i+1) << ". " << unlocked[i].name << " (Damage: " << unlocked[i].damage << ")\n";
        }
        int action;
        std::cin >> action;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        int dmg = player.attackEnemy(megaboss, action-1);
        std::cout << "You use " << unlocked[action-1].name << " and deal " << dmg << " damage!\n";
        if (megaboss.health > 0) {
            int enemyAtk = rand() % megaboss.getUnlockedMoves().size();
            int edmg = megaboss.attackEnemy(player, enemyAtk);
            std::cout << megaboss.name << " attacks you for " << edmg << " damage!\n";
        }
    }
    if (player.health > 0) {
        std::cout << "You have defeated the Ironbound Overlord and conquered the dungeon!\n";
    } else {
        std::cout << "You were defeated by the Ironbound Overlord...\n";
    }
}
