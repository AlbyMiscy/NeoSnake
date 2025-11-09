#include "engine.hpp"

// Relocate the fruit to a random position on the map,
// avoiding both snake segments and wall cells.
void Engine::moveFruit() {
    const float cell = map.getCellSize();

    size_t cols = static_cast<size_t>(map.getWorldWidth() / cell);
    size_t rows = static_cast<size_t>(map.getWorldHeight() / cell);

    if (cols == 0 || rows == 0)
        return;

    Vector2f newPos;
    bool valid = false;

    // Random engine
    srand(static_cast<unsigned int>(time(nullptr)));

    // Try until a valid position is found
    while (!valid) {
        size_t cx = rand() % cols;
        size_t cy = rand() % rows;

        // Skip this cell if it's a wall
        if (map.isWallCell(cx, cy))
            continue;

        // Convert cell coordinates to world coordinates
        newPos = Vector2f(static_cast<float>(cx) * cell,
                          static_cast<float>(cy) * cell);

        // Check collision with the snake body
        bool overlapsSnake = false;
        for (auto& section : snake) {
            if (section.getPosition() == newPos) {
                overlapsSnake = true;
                break;
            }
        }

        // If it doesn’t overlap the snake, it's valid
        if (!overlapsSnake)
            valid = true;
    }

    fruit.setPosition(newPos);
    fruit.pickRandomFrame();
}