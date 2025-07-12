#pragma once
#include <string>
#include <vector>
#include "Character.h"

class Enemy {
public:
    static std::vector<Enemy> loadEnemiesFromJSON(const std::string& filename);
    std::string name;
    std::string type; // e.g. Monster, Beast, Undead, Boss, etc.
    int health;
    int baseDamage;
    int defense;
    int speed;
    int level;
    std::vector<Move> moves;
    std::string spriteFolder;
    std::vector<std::string> traits; // e.g. "Flying", "Poisonous", etc.
    std::string tag; // "common", "elite", "miniboss", "boss"

    Enemy();
    Enemy(const std::string& name, const std::string& type, int health, int baseDamage, int defense, int speed, int level,
          const std::vector<Move>& moves = {}, const std::string& spriteFolder = "", const std::vector<std::string>& traits = {}, const std::string& tag = "common");
};
