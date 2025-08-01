#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    void setPosition(float x, float y);

private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float speed;

    void handleInput(float deltaTime);
};