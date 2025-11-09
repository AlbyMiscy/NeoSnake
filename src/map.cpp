#include "map.hpp"

Map::Map(float cellSize) : grid(), cellSize(cellSize) {
    tile.setSize(Vector2f(cellSize, cellSize));
    tile.setFillColor(Color(100, 100, 100)); // light gray
}

void Map::CreateFromImage(const Image& img){
    grid.clear(); 
    grid = vector(img.getSize().x, vector(img.getSize().y, 0));

    for(size_t x = 0; x < grid.size(); x++){
        for(size_t y = 0; y < grid[x].size(); y++){
            Color color = img.getPixel(Vector2u(static_cast<unsigned int>(x), static_cast<unsigned int>(y)));
            if(color == Color::Black){
                grid[x][y] = 1;
            }
        }
    }
}

void Map::Draw(RenderWindow& window){
    if(tileTexture) tile.setTexture(tileTexture);
    else tile.setTexture(nullptr);

    for (size_t x = 0; x < grid.size(); ++x) {
        for (size_t y = 0; y < grid[x].size(); ++y) {
            if (grid[x][y]) {
                tile.setPosition(Vector2f(
                                    static_cast<float>(x) * cellSize,
                                    static_cast<float>(y) * cellSize
                                ));
                window.draw(tile);
            }
        }
    }
}

float Map::getWorldWidth() const {
    return grid.empty() ? 0.f : static_cast<float>(grid.size()) * cellSize;
}

float Map::getWorldHeight() const {
    return grid.empty() ? 0.f : static_cast<float>(grid[0].size()) * cellSize;
}

bool Map::isWallCell(size_t cx, size_t cy) const {
    if (cx >= grid.size()) return false;
    if (cy >= grid[cx].size()) return false;
    return grid[cx][cy] != 0;
}

Vector2i Map::worldToCell(const Vector2f& p) const {
    int cx = int(floor(p.x / cellSize));
    int cy = int(floor(p.y / cellSize));
    return { cx, cy };
}