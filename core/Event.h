#pragma once
#include "Character.h"
#include <vector>
#include <string>
#include <functional>

struct ChoiceOption {
    std::string description;
    std::function<void(Character&)> effect;
};

struct ChoiceEvent {
    std::string flavorText;
    std::vector<ChoiceOption> options;
};

class Event {
public:
    static void trigger(Character& player, int floor);
    static void miniBoss(Character& player, int floor);
    static void megaBoss(Character& player);
    static const std::vector<ChoiceEvent> CHOICE_EVENTS;
};
