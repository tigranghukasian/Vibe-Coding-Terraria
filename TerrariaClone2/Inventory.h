#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Inventory {
public:
    Inventory();
    void addItem(int itemType, int count = 1);
    bool removeItem(int itemType, int count = 1);
    int getSelectedItem() const;
    int getItemCount(int itemType) const;
    void selectSlot(int slot);
    void draw(sf::RenderWindow& window);
    void handleInput();

private:
    struct ItemSlot {
        int itemType;
        int count;
        ItemSlot() : itemType(0), count(0) {}
    };

    std::vector<ItemSlot> slots;
    int selectedSlot;
    int maxSlots;

    // UI elements
    sf::RectangleShape slotBackground;
    sf::RectangleShape selectedSlotHighlight;
    sf::Font font;
    sf::Text countText;

    void loadFont();
    sf::Color getItemColor(int itemType) const;
};