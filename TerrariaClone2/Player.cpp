#include "Player.h"
#include "World.h"

Player::Player() : position(400, 200), velocity(0, 0), speed(200.0f), onGround(false) {
    // Try to load player texture, fall back to colored rectangle if fails
    if (!texture.loadFromFile("assets/player.png")) {
        // Create a simple colored texture if image doesn't exist
        sf::Image image;
        image.create(24, 40, sf::Color::Red);
        texture.loadFromImage(image);
    }

    sprite.setTexture(texture);
    sprite.setOrigin(12.0f, 20.0f);
    sprite.setPosition(position);
}

void Player::update(float deltaTime, const World& world) {
    // Apply gravity
    const float GRAVITY = 800.0f; // Pixels per second squared
    velocity.y += GRAVITY * deltaTime;

    // Handle input
    handleInput(deltaTime, world);

    // Apply physics movement
    applyPhysics(deltaTime, world);

    sprite.setPosition(position);
}

void Player::handleInput(float deltaTime, const World& world) {
    const float JUMP_SPEED = 350.0f;

    // Horizontal movement
    velocity.x = 0; // Reset horizontal velocity

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x = -speed;
        sprite.setScale(-1.0f, 1.0f);
        sprite.setPosition(position.x + 24, position.y); // Offset by width when flipped
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x = speed;
        sprite.setScale(1.0f, 1.0f);
        sprite.setPosition(position.x, position.y); // Normal position
    }

    // Jumping (only if on ground)
    if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && onGround) {
        velocity.y = -JUMP_SPEED; // Negative Y = upward
        onGround = false;
    }
}

void Player::applyPhysics(float deltaTime, const World& world) {
    // Test horizontal movement
    sf::Vector2f newPosition = position;
    newPosition.x += velocity.x * deltaTime;
    sf::Vector2f collisionPos = sf::Vector2f(newPosition.x - 12, newPosition.y - 20);

    if (!world.checkCollision(collisionPos, getSize())) {
        position.x = newPosition.x;
    }

    // Test vertical movement  
    newPosition = position;
    newPosition.y += velocity.y * deltaTime;
    collisionPos = sf::Vector2f(newPosition.x - 12, newPosition.y - 20);

    if (!world.checkCollision(collisionPos, getSize())) {
        position.y = newPosition.y;
        onGround = false;
    }
    else {
        if (velocity.y > 0) {
            onGround = true;
            velocity.y = 0;
        }
        else {
            velocity.y = 0;
        }
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