#include "Character.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "../external/json.hpp"
using json = nlohmann::json;

Character::Character()
    : name(""), charClass(""), health(0), maxHealth(0), baseDamage(0), defense(0), speed(0), xp(0), level(1), spriteFolder("") {}

Character::Character(const std::string& n, const std::string& c, int h, int mh, int bd, int d, int s, int x, int lvl,
                     const std::vector<std::string>& inv, const std::vector<std::string>& eq, const std::string& spriteFolder)
    : name(n), charClass(c), health(h), maxHealth(mh), baseDamage(bd), defense(d), speed(s), xp(x), level(lvl), inventory(inv), equipped(eq), spriteFolder(spriteFolder) {}

void Character::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

int Character::attackEnemy(Character& target, int moveIndex) {
    auto unlocked = getUnlockedMoves();
    if (moveIndex < 0 || moveIndex >= (int)unlocked.size()) moveIndex = 0;
    int damage = std::max(1, unlocked[moveIndex].damage + baseDamage - target.defense);
    target.takeDamage(damage);
    return damage;
}

void Character::gainXP(int amount) {
    xp += amount;
    while (xp >= level * 30) {
        xp -= level * 30;
        levelUp();
    }
}

void Character::levelUp() {
    level++;
    health += 20;
    baseDamage += 5;
    defense += 2;
}

std::vector<Move> Character::getUnlockedMoves() const {
    std::vector<Move> unlocked;
    for (const auto& move : moves) {
        if (level >= move.unlockLevel) unlocked.push_back(move);
    }
    // Limit to 4 moves (could prompt for swap logic)
    if (unlocked.size() > 4) unlocked.resize(4);
    return unlocked;
}

std::vector<Character> Character::loadCharactersFromJSON(const std::string& filename) {
    std::ifstream file(filename);
    json data;
    file >> data;
    std::vector<Character> characters;
    for (const auto& c : data) {
        std::vector<std::string> inv, eq;
        if (c.contains("inventory")) {
            for (const auto& item : c["inventory"]) inv.push_back(item);
        }
        if (c.contains("equipped")) {
            for (const auto& item : c["equipped"]) eq.push_back(item);
        }
        std::string spriteFolder = c.contains("spriteFolder") ? c["spriteFolder"].get<std::string>() : "";
        int maxHealth = c.contains("maxHealth") ? c["maxHealth"].get<int>() : (c.contains("health") ? c["health"].get<int>() : 100);
        int health = c.contains("health") ? c["health"].get<int>() : maxHealth;
        Character character(
            c["name"],
            c["class"],
            health,
            maxHealth,
            c["baseDamage"],
            c["defense"],
            c["speed"],
            0, 1,
            inv, eq,
            spriteFolder
        );
        for (const auto& m : c["moves"]) {
            Move move;
            move.name = m["name"];
            move.damage = m["damage"];
            move.type = m["type"];
            move.unlockLevel = m["unlockLevel"];
            character.moves.push_back(move);
        }
        // Auto-equip up to 4 moves that are unlocked for the character's level
        character.equippedMoves.clear();
        for (const auto& move : character.moves) {
            if ((int)character.equippedMoves.size() >= 4) break;
            if (character.level >= move.unlockLevel) {
                character.equippedMoves.push_back(move);
            }
        }
        characters.push_back(character);
    }
    return characters;
}

// Equip an item (move from inventory to equipped)
bool Character::equipItem(const std::string& item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        equipped.push_back(item);
        inventory.erase(it);
        return true;
    }
    return false;
}

// Unequip an item (move from equipped to inventory)
bool Character::unequipItem(const std::string& item) {
    auto it = std::find(equipped.begin(), equipped.end(), item);
    if (it != equipped.end()) {
        inventory.push_back(item);
        equipped.erase(it);
        return true;
    }
    return false;
}

// Equip a move (if less than 4 equipped)
bool Character::equipMove(const Move& move) {
    if (equippedMoves.size() < 4) {
        equippedMoves.push_back(move);
        return true;
    }
    return false;
}

// Replace an equipped move
bool Character::replaceEquippedMove(int index, const Move& newMove) {
    if (index >= 0 && index < 4 && equippedMoves.size() == 4) {
        equippedMoves[index] = newMove;
        return true;
    }
    return false;
}
