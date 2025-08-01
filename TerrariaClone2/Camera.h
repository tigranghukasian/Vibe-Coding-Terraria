#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(float designWidth, float designHeight);
    void followPlayer(sf::Vector2f playerPosition);
    void setWorldBounds(float worldWidth, float worldHeight);
    void handleResize(sf::Vector2u newSize); // Add this method
    sf::View getView() const;
    sf::Vector2f getOffset() const;

private:
    sf::View view;
    sf::Vector2f offset;
    float designWidth;  // Original design resolution
    float designHeight;
    float worldWidth;
    float worldHeight;

    void updateView();
};