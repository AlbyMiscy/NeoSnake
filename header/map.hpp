#ifndef SNAKE_MAP_HPP
#define SNAKE_MAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

using namespace sf;
using namespace std;

class Map {
    private:
        vector<vector<int>> grid;
        float cellSize;
        RectangleShape tile;
        const Texture* tileTexture = nullptr;

    public:
        explicit Map(float cellSize = 20.0f);

        void CreateFromImage(const Image& img);
        void Draw(RenderWindow& window);

        void SetTileTexture(const Texture* tex) { tileTexture = tex; };
        float getCellSize() const { return cellSize; }

        float getWorldWidth() const;
        float getWorldHeight() const;
        bool isWallCell(size_t cx, size_t cy) const;
        Vector2i worldToCell(const Vector2f& p) const;
};

#endif // SNAKE_MAP_HPP