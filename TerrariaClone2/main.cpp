#include <SFML/Graphics.hpp>
#include "Player.h"
#include "World.h"
#include "Camera.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Terraria Game");
    window.setFramerateLimit(60);

    // Create game objects
    World world(50, 30, 32); // Bigger world: 50x30 tiles, 32 pixels each
    Player player;
    Camera camera(800, 600);

    // Set camera world bounds
    camera.setWorldBounds(world.getWorldWidth(), world.getWorldHeight());

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Update game objects
        player.update(deltaTime);
        camera.followPlayer(player.getPosition());

        // Set camera view
        window.setView(camera.getView());

        // Render everything
        window.clear(sf::Color(135, 206, 235)); // Sky blue
        world.draw(window, camera.getOffset());
        player.draw(window);
        window.display();
    }

    return 0;
}