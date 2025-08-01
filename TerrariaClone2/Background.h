#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Background {
public:
    Background(int screenWidth, int screenHeight);
    void draw(sf::RenderWindow& window, sf::Vector2f cameraOffset);

    // Manual offset controls
    void setMountainOffset(float x, float y) { mountainOffsetX = x; mountainOffsetY = y; }
    void setTreeOffset(float x, float y) { treeOffsetX = x; treeOffsetY = y; }

private:
    struct BackgroundLayer {
        sf::Texture texture;
        sf::Sprite sprite;
        float parallaxSpeed;
        float scale;
    };

    BackgroundLayer mountains;
    BackgroundLayer trees;

    int screenWidth;
    int screenHeight;

    // Manual offsets
    float mountainOffsetX = 0.0f;
    float mountainOffsetY = 0.0f;
    float treeOffsetX = 0.0f;
    float treeOffsetY = 0.0f;

    bool loadTextures();
};