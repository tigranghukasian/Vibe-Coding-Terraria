#include "Background.h"
#include <cmath>
#include <iostream>

Background::Background(int screenWidth, int screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {

    if (!loadTextures()) {
        std::cout << "Warning: Could not load background textures. Make sure 'assets/mountains.png' and 'assets/trees.png' exist." << std::endl;
    }

    // Set up mountain layer (slowest parallax)
    mountains.parallaxSpeed = 0.2f;
    mountains.scale = static_cast<float>(screenHeight) / 1000.0f; // Scale to fit screen height
    mountains.sprite.setTexture(mountains.texture);
    mountains.sprite.setScale(mountains.scale, mountains.scale);

    // Set up tree layer (faster parallax)
    trees.parallaxSpeed = 0.5f;
    trees.scale = static_cast<float>(screenHeight) / 1000.0f; // Scale to fit screen height
    trees.sprite.setTexture(trees.texture);
    trees.sprite.setScale(trees.scale, trees.scale);
}

bool Background::loadTextures() {
    bool success = true;

    // Try to load mountain texture
    if (!mountains.texture.loadFromFile("assets/mountains.png")) {
        // Create fallback mountain texture if file doesn't exist
        sf::Image mountainImage;
        mountainImage.create(1000, 1000, sf::Color::Transparent);

        // Simple mountain fallback - blue gradient triangles
        for (int x = 0; x < 1000; x++) {
            float mountain1 = sin(x * 0.01f) * 200 + 300;
            float mountain2 = sin(x * 0.007f + 2) * 150 + 250;

            int maxHeight = static_cast<int>(std::max(mountain1, mountain2));

            for (int y = 1000 - maxHeight; y < 1000; y++) {
                if (y >= 0) {
                    sf::Uint8 blueValue = static_cast<sf::Uint8>(60 + (y * 60 / 1000));
                    mountainImage.setPixel(x, y, sf::Color(30, 40, blueValue, 180));
                }
            }
        }

        mountains.texture.loadFromImage(mountainImage);
        success = false;
    }

    // Try to load tree texture
    if (!trees.texture.loadFromFile("assets/trees.png")) {
        // Create fallback tree texture if file doesn't exist
        sf::Image treeImage;
        treeImage.create(1000, 1000, sf::Color::Transparent);

        // Simple tree fallback - green triangular shapes
        for (int x = 0; x < 1000; x += 60) {
            int treeHeight = 200 + (x % 100);

            // Draw tree trunk
            for (int tx = x - 5; tx < x + 5 && tx < 1000; tx++) {
                for (int ty = 800; ty < 1000; ty++) {
                    if (tx >= 0) {
                        treeImage.setPixel(tx, ty, sf::Color(60, 40, 20, 200));
                    }
                }
            }

            // Draw tree crown
            for (int ty = 1000 - treeHeight; ty < 800 && ty >= 0; ty++) {
                int crownWidth = static_cast<int>((ty - (1000 - treeHeight)) * 60 / treeHeight);
                for (int tx = x - crownWidth / 2; tx < x + crownWidth / 2 && tx < 1000; tx++) {
                    if (tx >= 0) {
                        sf::Uint8 green = static_cast<sf::Uint8>(80 + (tx % 30));
                        treeImage.setPixel(tx, ty, sf::Color(20, green, 30, 220));
                    }
                }
            }
        }

        trees.texture.loadFromImage(treeImage);
        success = false;
    }

    return success;
}

void Background::draw(sf::RenderWindow& window, sf::Vector2f cameraOffset) {
    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();

    // Get actual texture sizes
    sf::Vector2u mountainTextureSize = mountains.texture.getSize();
    sf::Vector2u treeTextureSize = trees.texture.getSize();

    // Calculate actual width after scaling
    float mountainWidth = mountainTextureSize.x * mountains.scale;
    float treeWidth = treeTextureSize.x * trees.scale;

    // Calculate parallax positions
    float mountainParallaxX = cameraOffset.x * mountains.parallaxSpeed;
    float treeParallaxX = cameraOffset.x * trees.parallaxSpeed;

    // Find starting positions for seamless tiling
    float screenLeft = viewCenter.x - viewSize.x / 2;
    float mountainStartX = screenLeft - mountainWidth - fmod(mountainParallaxX, mountainWidth);
    float treeStartX = screenLeft - treeWidth - fmod(treeParallaxX, treeWidth);

    // Calculate how many textures needed
    int mountainCount = static_cast<int>((viewSize.x + mountainWidth * 2) / mountainWidth) + 2;
    int treeCount = static_cast<int>((viewSize.x + treeWidth * 2) / treeWidth) + 2;

    // Position on horizon line (center of screen) + manual offsets
    float horizonY = viewCenter.y;

    // Draw mountains - position so bottom of texture is on horizon + offsets
    for (int i = 0; i < mountainCount; i++) {
        mountains.sprite.setPosition(
            mountainStartX + i * mountainWidth + mountainOffsetX,
            horizonY - mountainTextureSize.y * mountains.scale + mountainOffsetY
        );
        window.draw(mountains.sprite);
    }

    // Draw trees - position so bottom of texture is on horizon + offsets
    for (int i = 0; i < treeCount; i++) {
        trees.sprite.setPosition(
            treeStartX + i * treeWidth + treeOffsetX,
            horizonY - treeTextureSize.y * trees.scale + treeOffsetY
        );
        window.draw(trees.sprite);
    }
}