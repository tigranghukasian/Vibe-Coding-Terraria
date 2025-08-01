#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class World {
public:
    World(int width, int height, int tileSize);
    void generateWorld();
    void draw(sf::RenderWindow& window, sf::Vector2f cameraOffset);
    int getWorldWidth() const { return worldWidth * tileSize; }
    int getWorldHeight() const { return worldHeight * tileSize; }
    int getSurfaceHeight(int x) const;

    // Collision detection methods
    bool isSolid(int tileX, int tileY) const;
    bool checkCollision(sf::Vector2f position, sf::Vector2f size) const;

    // Block interaction methods
    int breakBlock(int tileX, int tileY);
    bool placeBlock(int tileX, int tileY, int blockType);
    int getTileAt(int tileX, int tileY) const;

    // Convert world position to tile coordinates
    sf::Vector2i worldToTile(sf::Vector2f worldPos) const;

    // Grass management
    void updateGrassLayer();

    static const int AIR = 0;
    static const int DIRT = 1;
    static const int STONE = 2;
    static const int GRASS = 3;  // New grass block type

private:
    int worldWidth;
    int worldHeight;
    int tileSize;
    std::vector<std::vector<int>> tiles;

    sf::Texture dirtTexture;
    sf::Texture stoneTexture;
    sf::Texture grassTexture;  // New grass texture
    sf::Sprite tileSprite;

    void loadTextures();
};