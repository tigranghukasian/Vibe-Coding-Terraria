#pragma once
#include <SFML/Graphics.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);
    void followPlayer(sf::Vector2f playerPosition);
    void setWorldBounds(float worldWidth, float worldHeight);
    sf::View getView() const;
    sf::Vector2f getOffset() const;

private:
    sf::View view;
    sf::Vector2f offset;
    float screenWidth;
    float screenHeight;
    float worldWidth;
    float worldHeight;

    void updateView();
};