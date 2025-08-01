#include "World.h"

World::World(int width, int height, int tileSize)
    : worldWidth(width), worldHeight(height), tileSize(tileSize) {
    tiles.resize(worldWidth, std::vector<int>(worldHeight));
    loadTextures();
    generateWorld();
}

void World::loadTextures() {
    // Try to load textures, create colored fallbacks if they don't exist
    if (!dirtTexture.loadFromFile("assets/dirt.png")) {
        sf::Image dirtImage;
        dirtImage.create(tileSize, tileSize, sf::Color(139, 69, 19)); // Brown
        dirtTexture.loadFromImage(dirtImage);
    }

    if (!stoneTexture.loadFromFile("assets/stone.png")) {
        sf::Image stoneImage;
        stoneImage.create(tileSize, tileSize, sf::Color(128, 128, 128)); // Gray
        stoneTexture.loadFromImage(stoneImage);
    }

    float tileScale = 32.0f / 1024.0f; // = 0.03125f
    tileSprite.setScale(tileScale, tileScale);
}

void World::generateWorld() {
    for (int x = 0; x < worldWidth; x++) {
        for (int y = 0; y < worldHeight; y++) {
            if (y < 8) {
                tiles[x][y] = AIR;
            }
            else if (y < 12) {
                tiles[x][y] = DIRT;
            }
            else {
                tiles[x][y] = STONE;
            }
        }
    }
}

void World::draw(sf::RenderWindow& window, sf::Vector2f cameraOffset) {
    // Only draw tiles that are visible on screen for better performance
    int startX = std::max(0, static_cast<int>(cameraOffset.x / tileSize));
    int endX = std::min(worldWidth, static_cast<int>((cameraOffset.x + 800) / tileSize) + 1);
    int startY = std::max(0, static_cast<int>(cameraOffset.y / tileSize));
    int endY = std::min(worldHeight, static_cast<int>((cameraOffset.y + 600) / tileSize) + 1);

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            if (tiles[x][y] != AIR) {
                tileSprite.setPosition(x * tileSize, y * tileSize);

                if (tiles[x][y] == DIRT) {
                    tileSprite.setTexture(dirtTexture);
                }
                else if (tiles[x][y] == STONE) {
                    tileSprite.setTexture(stoneTexture);
                }

                window.draw(tileSprite);
            }
        }
    }
}