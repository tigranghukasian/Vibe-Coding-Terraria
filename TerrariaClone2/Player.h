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
    sf::Vector2f velocity; // Add velocity for physics
    float speed;
    bool onGround; // Track if player is on ground

    void handleInput(float deltaTime, const class World& world);
    void applyPhysics(float deltaTime, const class World& world); // Add physics method
    sf::Vector2f getCollisionPosition() const { return sf::Vector2f(position.x - 12, position.y - 20); }
};