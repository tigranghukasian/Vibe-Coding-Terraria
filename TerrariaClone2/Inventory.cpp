#include "Inventory.h"
#include "World.h"

Inventory::Inventory() : selectedSlot(0), maxSlots(9) {
    slots.resize(maxSlots);

    // Set up UI elements
    slotBackground.setSize(sf::Vector2f(40, 40));
    slotBackground.setFillColor(sf::Color(50, 50, 50, 200));
    slotBackground.setOutlineThickness(2);
    slotBackground.setOutlineColor(sf::Color::Black);

    selectedSlotHighlight.setSize(sf::Vector2f(40, 40));
    selectedSlotHighlight.setFillColor(sf::Color::Transparent);
    selectedSlotHighlight.setOutlineThickness(3);
    selectedSlotHighlight.setOutlineColor(sf::Color::Yellow);

    loadFont();
    countText.setFont(font);
    countText.setCharacterSize(24);
    countText.setFillColor(sf::Color::White);
    countText.setOutlineColor(sf::Color::Black);
    countText.setOutlineThickness(2);
    countText.setStyle(sf::Text::Bold); // Make it bold for better visibility
}

void Inventory::loadFont() {
    // Try to load a font, create a simple fallback if it fails
    if (!font.loadFromFile("assets/font.ttf")) {
        // Font loading failed, text won't display but the rest will work
    }
}

void Inventory::addItem(int itemType, int count) {
    if (itemType == World::AIR) return;

    // First try to add to existing stack
    for (auto& slot : slots) {
        if (slot.itemType == itemType) {
            slot.count += count;
            return;
        }
    }

    // Find empty slot
    for (auto& slot : slots) {
        if (slot.itemType == World::AIR || slot.count == 0) {
            slot.itemType = itemType;
            slot.count = count;
            return;
        }
    }
}

bool Inventory::removeItem(int itemType, int count) {
    for (auto& slot : slots) {
        if (slot.itemType == itemType && slot.count >= count) {
            slot.count -= count;
            if (slot.count <= 0) {
                slot.itemType = World::AIR;
                slot.count = 0;
            }
            return true;
        }
    }
    return false;
}

int Inventory::getSelectedItem() const {
    if (selectedSlot >= 0 && selectedSlot < maxSlots && slots[selectedSlot].count > 0) {
        return slots[selectedSlot].itemType;
    }
    return World::AIR;
}

int Inventory::getItemCount(int itemType) const {
    for (const auto& slot : slots) {
        if (slot.itemType == itemType) {
            return slot.count;
        }
    }
    return 0;
}

void Inventory::selectSlot(int slot) {
    if (slot >= 0 && slot < maxSlots) {
        selectedSlot = slot;
    }
}

void Inventory::handleInput() {
    // Number keys 1-9 for slot selection
    for (int i = 0; i < 9; i++) {
        if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i))) {
            selectSlot(i);
            break;
        }
    }
}

sf::Color Inventory::getItemColor(int itemType) const {
    switch (itemType) {
    case World::DIRT: return sf::Color(139, 69, 19);   // Brown
    case World::STONE: return sf::Color(128, 128, 128); // Gray
    case World::GRASS: return sf::Color(34, 139, 34);   // Forest green
    default: return sf::Color::White;
    }
}

void Inventory::draw(sf::RenderWindow& window) {
    sf::Vector2f viewSize = window.getView().getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();

    // Position inventory at bottom center of screen
    float startX = viewCenter.x - (maxSlots * 45) / 2;
    float y = viewCenter.y + viewSize.y / 2 - 60;

    for (int i = 0; i < maxSlots; i++) {
        float x = startX + i * 45;

        // Draw slot background
        slotBackground.setPosition(x, y);
        window.draw(slotBackground);

        // Draw selection highlight
        if (i == selectedSlot) {
            selectedSlotHighlight.setPosition(x, y);
            window.draw(selectedSlotHighlight);
        }

        // Draw item if slot has items
        if (slots[i].count > 0) {
            sf::RectangleShape itemRect(sf::Vector2f(30, 30));
            itemRect.setPosition(x + 5, y + 5);
            itemRect.setFillColor(getItemColor(slots[i].itemType));
            window.draw(itemRect);

            // Draw count if more than 1
            if (slots[i].count > 1) {
                countText.setString(std::to_string(slots[i].count));
                // Position text in bottom-right corner of slot
                countText.setPosition(x + 24, y + 16);
                window.draw(countText);
            }
        }
    }
}