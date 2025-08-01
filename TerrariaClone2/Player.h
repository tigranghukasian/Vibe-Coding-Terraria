#pragma once
#include <SFML/Graphics.hpp>
#include "Inventory.h"

class Player {
public:
    Player();
    void update(float deltaTime, class World& world, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const { return sf::Vector2f(24, 40); }
    void setPosition(float x, float y);
    Inventory& getInventory() { return inventory; }

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float speed;
    bool onGround;
    Inventory inventory;

    // Mouse interaction
    sf::RectangleShape targetHighlight;
    sf::Vector2i targetTile;
    float interactionRange;

    void handleInput(float deltaTime, class World& world);
    void handleMouseInput(class World& world, sf::RenderWindow& window);
    void applyPhysics(float deltaTime, class World& world);
    sf::Vector2f getCollisionPosition() const { return sf::Vector2f(position.x - 12, position.y - 20); }
    sf::Vector2f getMouseWorldPosition(sf::RenderWindow& window) const;
    void updateTargetTile(class World& world, sf::RenderWindow& window);
};