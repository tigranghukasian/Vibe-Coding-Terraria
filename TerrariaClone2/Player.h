#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(float deltaTime, const class World& world); // Add world parameter
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const { return sf::Vector2f(24, 40); } // Add size getter
    void setPosition(float x, float y);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float speed;

    void handleInput(float deltaTime, const class World& world);
};