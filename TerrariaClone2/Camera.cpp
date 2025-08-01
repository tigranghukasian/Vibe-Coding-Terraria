#include "Camera.h"
#include <algorithm>

Camera::Camera(float designWidth, float designHeight)
    : designWidth(designWidth), designHeight(designHeight), worldWidth(0), worldHeight(0) {
    view.setSize(designWidth, designHeight);
    view.setCenter(designWidth / 2, designHeight / 2);
    offset = sf::Vector2f(0, 0);
}

void Camera::followPlayer(sf::Vector2f playerPosition) {
    // Simply center camera on player without restrictive bounds
    view.setCenter(playerPosition.x, playerPosition.y);

    // Calculate offset for drawing using current view size
    sf::Vector2f viewSize = view.getSize();
    offset.x = playerPosition.x - viewSize.x / 2;
    offset.y = playerPosition.y - viewSize.y / 2;
}

void Camera::handleResize(sf::Vector2u newSize) {
    // Simply update the view size to match the window size
    // This removes letterboxing but allows everything to be visible
    view.setSize(static_cast<float>(newSize.x), static_cast<float>(newSize.y));

    // Reset viewport to full window
    view.setViewport(sf::FloatRect(0, 0, 1, 1));

    // Update our design dimensions to match
    designWidth = static_cast<float>(newSize.x);
    designHeight = static_cast<float>(newSize.y);
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