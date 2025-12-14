#include "engine.hpp"

// Relocate the fruit to a random position on the map,
// avoiding both snake segments, wall cells, and unreachable areas.
void Engine::moveFruit() {
    const float cell = levelMap.getCellSize();

    size_t cols = static_cast<size_t>(levelMap.getWorldWidth() / cell);
    size_t rows = static_cast<size_t>(levelMap.getWorldHeight() / cell);

    if (cols == 0 || rows == 0 || snake.empty())
        return;

    Vector2f newPos;
    bool valid = false;

    // Random engine
    srand(static_cast<unsigned int>(time(nullptr)));

    // Get snake head position in grid coordinates
    Vector2i snakeHeadCell = levelMap.worldToCell(snake.front().getPosition());
    Vector2u snakeHeadTile(static_cast<unsigned int>(snakeHeadCell.x), 
                           static_cast<unsigned int>(snakeHeadCell.y));

    // Helper class for pathfinding
    class MyGrid : public GridProvider {
        const Engine* engine;
    public:
        MyGrid(const Engine* e) : engine(e) {}
        
        Vector2u size() const override {
            return engine->mapSize;
        }
        
        bool isBlocked(Vector2u tile) const override {
            return engine->isBlocked(tile);
        }
    };
    
    MyGrid grid(this);

    // Try until a valid position is found (with a safety limit)
    int attempts = 0;
    const int maxAttempts = 1000;
    
    while (!valid && attempts < maxAttempts) {
        size_t cx = rand() % cols;
        size_t cy = rand() % rows;

        // Skip this cell if it's a wall
        if (levelMap.isWallCell(cx, cy)) {
            attempts++;
            continue;
        }

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

        if (overlapsSnake) {
            attempts++;
            continue;
        }

        // CRITICAL CHECK: verify that the fruit is reachable from snake head
        Vector2u fruitTile(static_cast<unsigned int>(cx), static_cast<unsigned int>(cy));
        deque<Vector2u> path = astar(grid, snakeHeadTile, fruitTile);
        
        // If a path exists, the fruit is reachable
        if (!path.empty()) {
            valid = true;
        }
        
        attempts++;
    }

    // Fallback: if no valid position found after many attempts, 
    // just place it anywhere non-wall (better than infinite loop)
    if (!valid) {
        for (size_t cy = 0; cy < rows && !valid; ++cy) {
            for (size_t cx = 0; cx < cols && !valid; ++cx) {
                if (!levelMap.isWallCell(cx, cy)) {
                    newPos = Vector2f(static_cast<float>(cx) * cell,
                                      static_cast<float>(cy) * cell);
                    valid = true;
                }
            }
        }
    }

    fruit.setPosition(newPos);
    fruit.pickRandomFrame();
}
