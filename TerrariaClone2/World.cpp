#include "World.h"
#include "PerlinNoise.h" 

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

    // Load grass texture - green on top, brown on bottom
    if (!grassTexture.loadFromFile("assets/grass.png")) {
        sf::Image grassImage;
        grassImage.create(tileSize, tileSize, sf::Color(139, 69, 19)); // Brown base

        // Add green top layer (top 1/4 of the block)
        for (int x = 0; x < tileSize; x++) {
            for (int y = 0; y < tileSize / 4; y++) {
                grassImage.setPixel(x, y, sf::Color(34, 139, 34)); // Forest green
            }
        }

        grassTexture.loadFromImage(grassImage);
    }

    float tileScale = 32.0f / 64.0f; // = 0.03125f
    tileSprite.setScale(tileScale, tileScale);
}

void World::generateWorld() {
    // Surface generation (keep your current surface code)
    for (int x = 0; x < worldWidth; x++) {
        double noiseValue = generateTerrain(x, 4, 0.5, 0.1);
        int surfaceHeight = static_cast<int>((noiseValue + 1.0) * 0.5 * (worldHeight * 0.4)) + (worldHeight * 0.3);
        surfaceHeight = std::max(5, std::min(surfaceHeight, worldHeight - 10));

        for (int y = 0; y < worldHeight; y++) {
            if (y < surfaceHeight) {
                tiles[x][y] = AIR;
            }
            else if (y < surfaceHeight + 5) {
                tiles[x][y] = DIRT;
            }
            else {
                tiles[x][y] = STONE;
            }
        }
    }

    // Cave generation - create 2D noise by combining x and y noise
    for (int x = 5; x < worldWidth - 5; x++) {
        for (int y = worldHeight / 2 + 3; y < worldHeight - 5; y++) {
            // Generate separate noise for x and y, then combine
            double xNoise = generateTerrain(x, 3, 0.6, 0.05);
            double yNoise = generateTerrain(y, 3, 0.6, 0.05);
            double caveNoise = (xNoise + yNoise) * 0.5; // Average them

            // Create caves where noise is above threshold
            if (caveNoise > 0.1 && tiles[x][y] != AIR) { // Lower threshold
                tiles[x][y] = AIR;
            }
        }
    }

    // Generate grass layer on top of dirt blocks
    updateGrassLayer();
}

void World::updateGrassLayer() {
    // Convert all surface dirt blocks to grass
    for (int x = 0; x < worldWidth; x++) {
        for (int y = 0; y < worldHeight; y++) {
            // If this is a dirt block with air above it, make it grass
            if (tiles[x][y] == DIRT) {
                bool hasAirAbove = (y == 0) || (y > 0 && tiles[x][y - 1] == AIR);
                if (hasAirAbove) {
                    tiles[x][y] = GRASS;
                }
            }
        }
    }
}

void World::draw(sf::RenderWindow& window, sf::Vector2f cameraOffset) {
    // Get the current view size instead of hardcoded 800x600
    sf::Vector2f viewSize = window.getView().getSize();

    // Only draw tiles that are visible on screen for better performance
    int startX = std::max(0, static_cast<int>(cameraOffset.x / tileSize));
    int endX = std::min(worldWidth, static_cast<int>((cameraOffset.x + viewSize.x) / tileSize) + 1);
    int startY = std::max(0, static_cast<int>(cameraOffset.y / tileSize));
    int endY = std::min(worldHeight, static_cast<int>((cameraOffset.y + viewSize.y) / tileSize) + 1);

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
                else if (tiles[x][y] == GRASS) {
                    tileSprite.setTexture(grassTexture);
                }

                window.draw(tileSprite);
            }
        }
    }
}

int World::getSurfaceHeight(int x) const {
    if (x < 0 || x >= worldWidth) return worldHeight / 2;

    for (int y = 0; y < worldHeight; y++) {
        if (tiles[x][y] != AIR) {
            return y * tileSize;
        }
    }
    return worldHeight / 2 * tileSize;
}

bool World::isSolid(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= worldWidth || tileY < 0 || tileY >= worldHeight) {
        return true;
    }
    return tiles[tileX][tileY] != AIR;
}

bool World::checkCollision(sf::Vector2f position, sf::Vector2f size) const {
    int leftTile = static_cast<int>(position.x / tileSize);
    int rightTile = static_cast<int>((position.x + size.x) / tileSize);
    int topTile = static_cast<int>(position.y / tileSize);
    int bottomTile = static_cast<int>((position.y + size.y) / tileSize);

    for (int x = leftTile; x <= rightTile; x++) {
        for (int y = topTile; y <= bottomTile; y++) {
            if (isSolid(x, y)) {
                return true;
            }
        }
    }
    return false;
}

int World::breakBlock(int tileX, int tileY) {
    if (tileX < 0 || tileX >= worldWidth || tileY < 0 || tileY >= worldHeight) {
        return AIR;
    }

    int brokenBlock = tiles[tileX][tileY];
    if (brokenBlock != AIR) {
        tiles[tileX][tileY] = AIR;

        // Update grass layer after breaking a block
        updateGrassLayer();

        // If we broke a grass block, return dirt instead
        if (brokenBlock == GRASS) {
            return DIRT;
        }
    }
    return brokenBlock;
}

bool World::placeBlock(int tileX, int tileY, int blockType) {
    if (tileX < 0 || tileX >= worldWidth || tileY < 0 || tileY >= worldHeight) {
        return false;
    }

    if (tiles[tileX][tileY] == AIR && blockType != AIR) {
        tiles[tileX][tileY] = blockType;

        // Update grass layer after placing a block
        updateGrassLayer();
        return true;
    }
    return false;
}

int World::getTileAt(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= worldWidth || tileY < 0 || tileY >= worldHeight) {
        return AIR;
    }
    return tiles[tileX][tileY];
}

sf::Vector2i World::worldToTile(sf::Vector2f worldPos) const {
    return sf::Vector2i(
        static_cast<int>(worldPos.x / tileSize),
        static_cast<int>(worldPos.y / tileSize)
    );
}