#include "Player.h"
#include "World.h"

Player::Player() : position(400, 200), velocity(0, 0), speed(200.0f), onGround(false), interactionRange(96.0f) {
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

    // Set up target highlight
    targetHighlight.setSize(sf::Vector2f(32, 32));
    targetHighlight.setFillColor(sf::Color::Transparent);
    targetHighlight.setOutlineThickness(2);
    targetHighlight.setOutlineColor(sf::Color(255, 255, 255, 128));

    targetTile = sf::Vector2i(-1, -1);
}

void Player::spawnOnSurface(World& world) {
    // Find a good spawn location near the center of the world
    int worldCenterX = world.getWorldWidth() / 2;

    // Find the surface height at the center of the world
    int surfaceY = world.getSurfaceHeight(worldCenterX / 32) / 32; // Convert to tile coordinates

    // Look for a suitable spawn location (flat area with at least 2 blocks of air above)
    int bestSpawnX = worldCenterX;
    int bestSpawnY = surfaceY;

    // Search in a range around the center for a flatter area
    for (int searchX = worldCenterX - 160; searchX <= worldCenterX + 160; searchX += 32) {
        if (searchX < 32 || searchX >= world.getWorldWidth() - 32) continue;

        int tileX = searchX / 32;
        int currentSurfaceY = world.getSurfaceHeight(searchX) / 32;

        // Check if this location has enough air above it (at least 3 blocks high)
        bool hasEnoughSpace = true;
        for (int checkY = currentSurfaceY - 3; checkY < currentSurfaceY; checkY++) {
            if (world.getTileAt(tileX, checkY) != World::AIR) {
                hasEnoughSpace = false;
                break;
            }
        }

        // Prefer locations that are closer to the original center and have good clearance
        if (hasEnoughSpace && abs(currentSurfaceY - surfaceY) <= 2) {
            bestSpawnX = searchX;
            bestSpawnY = currentSurfaceY;
            break;
        }
    }

    // Set player position on the surface with some clearance
    position.x = static_cast<float>(bestSpawnX);
    position.y = static_cast<float>((bestSpawnY - 2) * 32); // Spawn 2 blocks above surface

    // Reset velocity and set on ground
    velocity = sf::Vector2f(0, 0);
    onGround = false; // Let physics handle ground detection

    // Update sprite position
    sprite.setPosition(position);
}

void Player::update(float deltaTime, World& world, sf::RenderWindow& window) {
    // Apply gravity
    const float GRAVITY = 800.0f; // Pixels per second squared
    velocity.y += GRAVITY * deltaTime;

    // Handle input
    handleInput(deltaTime, world);
    handleMouseInput(world, window);
    updateTargetTile(world, window);

    // Apply physics movement
    applyPhysics(deltaTime, world);

    sprite.setPosition(position);
}

void Player::handleInput(float deltaTime, World& world) {
    const float JUMP_SPEED = 350.0f;

    // Handle inventory input
    inventory.handleInput();

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

void Player::handleMouseInput(World& world, sf::RenderWindow& window) {
    static bool leftPressed = false;
    static bool rightPressed = false;

    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool rightClick = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    // Handle left click (break blocks) - only on press, not hold
    if (leftClick && !leftPressed && targetTile.x != -1) {
        int brokenBlock = world.breakBlock(targetTile.x, targetTile.y);
        if (brokenBlock != World::AIR) {
            inventory.addItem(brokenBlock);
        }
    }

    // Handle right click (place blocks) - only on press, not hold
    if (rightClick && !rightPressed && targetTile.x != -1) {
        int selectedItem = inventory.getSelectedItem();
        if (selectedItem != World::AIR) {
            // Use the new placeBlock method that checks for player collision
            if (world.placeBlock(targetTile.x, targetTile.y, selectedItem, position, getSize())) {
                inventory.removeItem(selectedItem);
            }
        }
    }

    leftPressed = leftClick;
    rightPressed = rightClick;
}

void Player::updateTargetTile(World& world, sf::RenderWindow& window) {
    sf::Vector2f mouseWorld = getMouseWorldPosition(window);
    sf::Vector2i mouseTile = world.worldToTile(mouseWorld);

    // Check if mouse is within interaction range
    sf::Vector2f tileCenter(mouseTile.x * 32 + 16, mouseTile.y * 32 + 16);
    sf::Vector2f playerCenter = position;
    float distance = sqrt(pow(tileCenter.x - playerCenter.x, 2) + pow(tileCenter.y - playerCenter.y, 2));

    if (distance <= interactionRange) {
        targetTile = mouseTile;
        targetHighlight.setPosition(mouseTile.x * 32, mouseTile.y * 32);
    }
    else {
        targetTile = sf::Vector2i(-1, -1);
    }
}

sf::Vector2f Player::getMouseWorldPosition(sf::RenderWindow& window) const {
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    return window.mapPixelToCoords(mousePixel);
}

void Player::applyPhysics(float deltaTime, World& world) {
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

    // Draw target highlight if valid
    if (targetTile.x != -1) {
        window.draw(targetHighlight);
    }

    // Draw inventory
    inventory.draw(window);
}

sf::Vector2f Player::getPosition() const {
    return position;
}

void Player::setPosition(float x, float y) {
    position.x = x;
    position.y = y;
    sprite.setPosition(position);
}