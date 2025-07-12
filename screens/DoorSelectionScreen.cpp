#include "DoorSelectionScreen.h"
#include <sstream>
#include <algorithm>

DoorSelectionScreen::DoorSelectionScreen(sf::Font& fontRef)
    : font(fontRef), numDoors(4), hovered(-1), floorNumber(1), floorText(fontRef, "", 48) {}

void DoorSelectionScreen::start(int floorNum) {
    floorNumber = floorNum;
    if (floorNumber == 1) {
        numDoors = 1;
    } else if (floorNumber == 2) {
        numDoors = 2;
    } else if (floorNumber % 10 == 0 && floorNumber < 50) {
        numDoors = 1; // Mini-boss only
    } else if (floorNumber == 50) {
        numDoors = 1; // Final boss only
    } else {
        int roll = rand() % 100;
        if (roll < 70) numDoors = 3; // 70% chance
        else if (roll < 90) numDoors = 2; // 20% chance
        else numDoors = 4; // 10% chance
    }
    std::ostringstream oss;
    oss << "Floor " << floorNumber;
    floorText.setString(oss.str());
    floorText.setCharacterSize(48);
    floorText.setFillColor(sf::Color::White);
    auto bounds = floorText.getLocalBounds();
    float centerX = static_cast<float>(windowSize.x) / 2.f;
    floorText.setPosition(sf::Vector2f(centerX - bounds.size.x / 2.f, 80.f));
    generateDoors();
}

void DoorSelectionScreen::generateDoors() {
    doorRects.clear();
    doorTexts.clear();
    // Use windowSize set from main.cpp for layout
    float screenWidth = static_cast<float>(windowSize.x);
    float screenHeight = static_cast<float>(windowSize.y);
    float desiredHeight = std::min(420.f, (screenHeight - 120.f) * 0.7f); // 420px max, 70% of available height
    float desiredWidth = desiredHeight * 0.56f; // Doors are taller than wide
    float spacing = desiredHeight * 0.22f;
    float totalWidth = numDoors * desiredWidth + (numDoors - 1) * spacing;
    float startX = (screenWidth - totalWidth) / 2.f;
    float y = screenHeight * 0.38f;
    static const std::vector<std::pair<std::string, int>> weightedDoors = {
        {"Common Enemy", 40},
        {"Mystery", 25},
        {"Merchant", 8},
        {"Rest Site", 8},
        {"Elite", 6},
        {"Treasure", 5}
    };
    std::vector<std::string> chosenLabels;
    if (floorNumber == 1) {
        chosenLabels.push_back("Common Enemy");
    } else if (floorNumber == 2) {
        chosenLabels = {"Common Enemy", "Mystery"};
    } else if (numDoors == 1) {
        chosenLabels.push_back((floorNumber == 50) ? "Boss" : "Mini-Boss");
    } else {
        // Always include one Common Enemy
        chosenLabels.push_back("Common Enemy");
        std::vector<std::string> pool;
        for (const auto& p : weightedDoors) {
            if (p.first != "Common Enemy") {
                for (int i = 0; i < p.second; ++i) pool.push_back(p.first);
            }
        }
        // Shuffle pool for randomness
        for (int i = 1; i < numDoors; ++i) {
            if (pool.empty()) break;
            int idx = rand() % pool.size();
            // Prevent duplicate Rest Site/Merchant/Elite/Treasure on same floor
            while ((pool[idx] == "Rest Site" && std::count(chosenLabels.begin(), chosenLabels.end(), "Rest Site") > 0) ||
                   (pool[idx] == "Merchant" && std::count(chosenLabels.begin(), chosenLabels.end(), "Merchant") > 0) ||
                   (pool[idx] == "Elite" && std::count(chosenLabels.begin(), chosenLabels.end(), "Elite") > 0) ||
                   (pool[idx] == "Treasure" && std::count(chosenLabels.begin(), chosenLabels.end(), "Treasure") > 0)) {
                pool.erase(pool.begin() + idx);
                if (pool.empty()) break;
                idx = rand() % pool.size();
            }
            chosenLabels.push_back(pool[idx]);
            pool.erase(pool.begin() + idx);
        }
    }
    for (int i = 0; i < numDoors; ++i) {
        sf::RectangleShape rect(sf::Vector2f(desiredWidth, desiredHeight));
        rect.setPosition(sf::Vector2f(startX + i * (desiredWidth + spacing), y));
        rect.setFillColor(sf::Color(80, 60, 40));
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(hovered == i ? 6.f : 3.f);
        doorRects.push_back(rect);
        std::string label = (i < (int)chosenLabels.size()) ? chosenLabels[i] : "Door";
        sf::Text txt(font, label, 36);
        txt.setFillColor(sf::Color::White);
        // Center text horizontally on door
        float textWidth = txt.getLocalBounds().size.x;
        float textHeight = txt.getLocalBounds().size.y;
        float textX = rect.getPosition().x + (desiredWidth - textWidth) / 2.f;
        float textY = rect.getPosition().y + desiredHeight - textHeight - 18.f;
        txt.setPosition(sf::Vector2f(textX, textY));
        doorTexts.push_back(txt);
    }
}

void DoorSelectionScreen::draw(sf::RenderWindow& window) {
    window.draw(floorText);
    for (auto& rect : doorRects) window.draw(rect);
    for (auto& txt : doorTexts) window.draw(txt);
}

bool DoorSelectionScreen::handleEvent(const sf::Event& event, sf::RenderWindow& window, int& doorSelected) {
    if (event.is<sf::Event::MouseMoved>()) {
        hovered = -1;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        for (int i = 0; i < (int)doorRects.size(); ++i) {
            if (doorRects[i].getGlobalBounds().contains(worldPos)) {
                hovered = i;
                doorRects[i].setOutlineThickness(6.f);
            } else {
                doorRects[i].setOutlineThickness(3.f);
            }
        }
    }
    if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
            for (int i = 0; i < (int)doorRects.size(); ++i) {
                if (doorRects[i].getGlobalBounds().contains(worldPos)) {
                    doorSelected = i;
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::string> DoorSelectionScreen::getDoorLabels() const {
    std::vector<std::string> labels;
    for (const auto& txt : doorTexts) {
        labels.push_back(txt.getString());
    }
    return labels;
}
