#include <SFML/Graphics.hpp>
#include "Player.h"
#include "World.h"
#include "Camera.h"
#include "Background.h"  // Add this include

int main() {
    // Create resizable window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Terraria Game", sf::Style::Default);
    window.setFramerateLimit(60);

    // Create game objects with fixed design resolution
    const float DESIGN_WIDTH = 800.0f;
    const float DESIGN_HEIGHT = 600.0f;

    World world(150, 100, 32); // Bigger world: 150x100 tiles, 32 pixels each
    Player player;
    Camera camera(DESIGN_WIDTH, DESIGN_HEIGHT);
    Background background(static_cast<int>(DESIGN_WIDTH), static_cast<int>(DESIGN_HEIGHT)); // Add background

    // IMPORTANT: Spawn player on surface after world generation
    player.spawnOnSurface(world);

    // Set manual offsets for each layer (example values - adjust as needed)
    background.setMountainOffset(0, 350);   // Move mountains 50 pixels down
    background.setTreeOffset(0, 750);      // Move trees 100 pixels down

    // Set camera world bounds
    camera.setWorldBounds(world.getWorldWidth(), world.getWorldHeight());

    // Handle initial window size
    camera.handleResize(window.getSize());

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle window resize
            else if (event.type == sf::Event::Resized) {
                camera.handleResize(sf::Vector2u(event.size.width, event.size.height));
            }
        }

        // Update game objects
        player.update(deltaTime, world, window); // Pass window for mouse input
        camera.followPlayer(player.getPosition());

        // Set camera view
        window.setView(camera.getView());

        // Render everything
        window.clear(sf::Color(118, 186, 213)); // Sky blue
        background.draw(window, camera.getOffset()); // Draw background first
        world.draw(window, camera.getOffset());
        player.draw(window);
        window.display();
    }

    return 0;
}