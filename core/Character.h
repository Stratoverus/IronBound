#pragma once
#include <string>
#include <vector>

struct Move {
    std::string name;
    int damage;
    std::string type;
    int unlockLevel;
};

class Character {
public:
    std::string name;
    std::string charClass;
    int health;
    int maxHealth;
    int baseDamage;
    int defense;
    int speed;
    int xp;
    int level;
    std::vector<Move> moves; // All unlocked moves
    std::vector<Move> equippedMoves; // Only 4 equipped moves
    std::string spriteFolder;
    std::vector<std::string> inventory;
    std::vector<std::string> equipped;

    Character();
    Character(const std::string& n, const std::string& c, int h, int mh, int bd, int d, int s, int x = 0, int lvl = 1,
              const std::vector<std::string>& inv = {}, const std::vector<std::string>& eq = {}, const std::string& spriteFolder = "");
    void takeDamage(int amount);
    int attackEnemy(Character& target, int moveIndex = 0);
    void gainXP(int amount);
    void levelUp();
    std::vector<Move> getUnlockedMoves() const;
    std::vector<Move> getEquippedMoves() const { return equippedMoves; }
    // Equip a move (if less than 4 equipped)
    bool equipMove(const Move& move);
    // Replace an equipped move
    bool replaceEquippedMove(int index, const Move& newMove);
    // Equip an item (move from inventory to equipped)
    bool equipItem(const std::string& item);
    // Unequip an item (move from equipped to inventory)
    bool unequipItem(const std::string& item);

    static std::vector<Character> loadCharactersFromJSON(const std::string& filename);
};
