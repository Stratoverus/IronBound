#include "Enemy.h"

Enemy::Enemy()
    : name(""), type(""), health(0), baseDamage(0), defense(0), speed(0), level(1), spriteFolder("") {}

Enemy::Enemy(const std::string& name, const std::string& type, int health, int baseDamage, int defense, int speed, int level,
             const std::vector<Move>& moves, const std::string& spriteFolder, const std::vector<std::string>& traits)
    : name(name), type(type), health(health), baseDamage(baseDamage), defense(defense), speed(speed), level(level),
      moves(moves), spriteFolder(spriteFolder), traits(traits) {}
