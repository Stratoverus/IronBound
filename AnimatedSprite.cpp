#include "AnimatedSprite.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

AnimatedSprite::AnimatedSprite()
    : frameTime(0.15f), timer(0.f), currentFrame(0), sprite(nullptr) {
}

AnimatedSprite::AnimatedSprite(const std::vector<std::string>& framePaths, float frameTime)
    : frameTime(frameTime), timer(0.f), currentFrame(0) {
    for (const auto& path : framePaths) {
        sf::Texture tex;
        if (tex.loadFromFile(path)) {
            frames.push_back(std::move(tex));
        }
    }
    if (!frames.empty()) {
        sprite = new sf::Sprite(frames[0]);
        // Improved auto-slicing: detect variable-sized frames separated by transparent padding
        sf::Image image = frames[0].copyToImage();
        unsigned int width = image.getSize().x;
        unsigned int height = image.getSize().y;
        bool inFrame = false;
        unsigned int frameStartX = 0;
        for (unsigned int x = 0; x < width; ++x) {
            bool allTransparent = true;
            for (unsigned int y = 0; y < height; ++y) {
                if (image.getPixel(sf::Vector2u(x, y)).a != 0) {
                    allTransparent = false;
                    break;
                }
            }
            if (!inFrame && !allTransparent) {
                frameStartX = x;
                inFrame = true;
            } else if (inFrame && allTransparent) {
                // Find frame bounds (left, top, width, height)
                unsigned int frameEndX = x - 1;
                // Find top and bottom
                unsigned int top = height, bottom = 0;
                for (unsigned int fx = frameStartX; fx <= frameEndX; ++fx) {
                    for (unsigned int fy = 0; fy < height; ++fy) {
                        if (image.getPixel(sf::Vector2u(fx, fy)).a != 0) {
                            if (fy < top) top = fy;
                            if (fy > bottom) bottom = fy;
                        }
                    }
                }
                unsigned int frameW = frameEndX - frameStartX + 1;
                unsigned int frameH = (bottom >= top) ? (bottom - top + 1) : 0;
                detectedFrameRects.push_back(sf::IntRect({static_cast<int>(frameStartX), static_cast<int>(top)}, {static_cast<int>(frameW), static_cast<int>(frameH)}));
                inFrame = false;
            }
        }
        // Handle last frame if it goes to the end
        if (inFrame) {
            unsigned int frameEndX = width - 1;
            unsigned int top = height, bottom = 0;
            for (unsigned int fx = frameStartX; fx <= frameEndX; ++fx) {
                for (unsigned int fy = 0; fy < height; ++fy) {
                    if (image.getPixel(sf::Vector2u(fx, fy)).a != 0) {
                        if (fy < top) top = fy;
                        if (fy > bottom) bottom = fy;
                    }
                }
            }
            unsigned int frameW = frameEndX - frameStartX + 1;
            unsigned int frameH = (bottom >= top) ? (bottom - top + 1) : 0;
            detectedFrameRects.push_back(sf::IntRect({static_cast<int>(frameStartX), static_cast<int>(top)}, {static_cast<int>(frameW), static_cast<int>(frameH)}));
        }
        std::cout << "Detected frame rects for this sprite: ";
        for (size_t i = 0; i < detectedFrameRects.size(); ++i) {
            const auto& rect = detectedFrameRects[i];
            std::cout << "Frame " << i << ": [x=" << rect.position.x << ", y=" << rect.position.y << ", w=" << rect.size.x << ", h=" << rect.size.y << "] ";
        }
        std::cout << std::endl;
    }
}

void AnimatedSprite::update(float dt) {
    if (frames.size() <= 1 && detectedFrameRects.empty()) return;
    timer += dt;
    if (timer >= frameTime) {
        timer = 0.f;
        currentFrame = (currentFrame + 1) % getFrameCount();
        if (!detectedFrameRects.empty()) {
            sprite->setTextureRect(detectedFrameRects[currentFrame]);
        } else {
            sprite->setTexture(frames[currentFrame]);
        }
    }
}

void AnimatedSprite::draw(sf::RenderWindow& window, sf::Vector2f position) {
    if (!detectedFrameRects.empty()) {
        const auto& rect = detectedFrameRects[currentFrame];
        float xOffset = -rect.size.x / 2.0f * sprite->getScale().x;
        // Align bottom of frame to y position (root to ground)
        float yOffset = -rect.size.y * sprite->getScale().y;
        sprite->setPosition(sf::Vector2f(position.x + xOffset, position.y + yOffset));
        sprite->setTextureRect(rect);
    } else {
        sf::Vector2u texSize = sprite->getTexture().getSize();
        float xOffset = -texSize.x / 2.0f * sprite->getScale().x;
        float yOffset = -texSize.y * sprite->getScale().y;
        sprite->setPosition(sf::Vector2f(position.x + xOffset, position.y + yOffset));
    }
    window.draw(*sprite);
}

void AnimatedSprite::setScale(sf::Vector2f scale) {
    sprite->setScale(scale);
}

void AnimatedSprite::setFrameTime(float time) {
    frameTime = time;
}

int AnimatedSprite::getFrameCount() const {
    return detectedFrameRects.empty() ? static_cast<int>(frames.size()) : static_cast<int>(detectedFrameRects.size());
}

const std::vector<sf::IntRect>& AnimatedSprite::getFrameRects() const {
    return detectedFrameRects;
}
