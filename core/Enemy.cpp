
#include "Enemy.h"
#include <fstream>
#include "../external/json.hpp"
using json = nlohmann::json;

std::vector<Enemy> Enemy::loadEnemiesFromJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        // Could throw or log an error, but for now just return empty
        return {};
    }
    json data;
    file >> data;
    std::vector<Enemy> enemies;
    for (const auto& e : data) {
        std::vector<Move> moves;
        if (e.contains("moves")) {
            for (const auto& m : e["moves"]) {
                Move move;
                move.name = m.value("name", "");
                move.damage = m.value("damage", 0);
                move.type = m.value("type", "");
                move.unlockLevel = m.value("unlockLevel", 1);
                moves.push_back(move);
            }
        }
        std::vector<std::string> traits;
        if (e.contains("traits")) {
            for (const auto& t : e["traits"]) traits.push_back(t);
        }
        std::string spriteFolder = e.value("spriteFolder", "");
        int maxHealth = e.contains("maxHealth") ? e["maxHealth"].get<int>() : (e.contains("hp") ? e["hp"].get<int>() : (e.contains("health") ? e["health"].get<int>() : 100));
        int health = e.contains("hp") ? e["hp"].get<int>() : (e.contains("health") ? e["health"].get<int>() : maxHealth);
        Enemy enemy(
            e.value("name", ""),
            e.value("type", ""),
            health,
            maxHealth,
            e.value("baseDamage", 0),
            e.value("defense", 0),
            e.value("speed", 0),
            e.value("level", 1),
            moves,
            spriteFolder,
            traits,
            e.value("tag", "common")
        );
        enemies.push_back(enemy);
    }
    return enemies;
}

Enemy::Enemy()
    : name(""), type(""), health(0), maxHealth(0), baseDamage(0), defense(0), speed(0), level(1), spriteFolder(""), tag("common") {}

Enemy::Enemy(const std::string& name, const std::string& type, int health, int maxHealth, int baseDamage, int defense, int speed, int level,
             const std::vector<Move>& moves, const std::string& spriteFolder, const std::vector<std::string>& traits, const std::string& tag)
    : name(name), type(type), health(health), maxHealth(maxHealth), baseDamage(baseDamage), defense(defense), speed(speed), level(level),
      moves(moves), spriteFolder(spriteFolder), traits(traits), tag(tag) {}
