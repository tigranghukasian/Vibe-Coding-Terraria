#include "Player.h"
#include "World.h"

Player::Player() : position(400, 200), speed(200.0f) {
    // Try to load player texture, fall back to colored rectangle if fails
    if (!texture.loadFromFile("assets/player.png")) {
        // Create a simple colored texture if image doesn't exist
        sf::Image image;
        image.create(24, 40, sf::Color::Red);
        texture.loadFromImage(image);
    }

    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void Player::update(float deltaTime, const World& world) {
    handleInput(deltaTime, world);
    sprite.setPosition(position);
}

void Player::handleInput(float deltaTime, const World& world) {
    sf::Vector2f movement(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        movement.x = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        movement.x = speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        movement.y = -speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        movement.y = speed;
    }

    //position += movement * deltaTime;

    // Test horizontal movement
    sf::Vector2f newPosition = position;
    newPosition.x += movement.x * deltaTime;

    if (!world.checkCollision(newPosition, getSize())) {
        position.x = newPosition.x;
    }

    // Test vertical movement
    newPosition = position;
    newPosition.y += movement.y * deltaTime;

    if (!world.checkCollision(newPosition, getSize())) {
        position.y = newPosition.y;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::Vector2f Player::getPosition() const {
    return position;
}

void Player::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}