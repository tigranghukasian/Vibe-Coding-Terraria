#include "Camera.h"
#include <algorithm>

Camera::Camera(float screenWidth, float screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
    view.setSize(screenWidth, screenHeight);
    view.setCenter(screenWidth / 2, screenHeight / 2);
    offset = sf::Vector2f(0, 0);
}

void Camera::followPlayer(sf::Vector2f playerPosition) {
    // Center camera on player
    float cameraX = playerPosition.x;
    float cameraY = playerPosition.y;

    // Keep camera within world bounds if world bounds are set
    if (worldWidth > 0 && worldHeight > 0) {
        // Don't let camera go beyond left edge
        cameraX = std::max(cameraX, screenWidth / 2);
        // Don't let camera go beyond right edge
        cameraX = std::min(cameraX, worldWidth - screenWidth / 2);

        // Don't let camera go beyond top edge
        cameraY = std::max(cameraY, screenHeight / 2);
        // Don't let camera go beyond bottom edge
        cameraY = std::min(cameraY, worldHeight - screenHeight / 2);
    }

    view.setCenter(cameraX, cameraY);

    // Calculate offset for drawing
    offset.x = cameraX - screenWidth / 2;
    offset.y = cameraY - screenHeight / 2;
}

void Camera::setWorldBounds(float worldWidth, float worldHeight) {
    this->worldWidth = worldWidth;
    this->worldHeight = worldHeight;
}

sf::View Camera::getView() const {
    return view;
}

sf::Vector2f Camera::getOffset() const {
    return offset;
}