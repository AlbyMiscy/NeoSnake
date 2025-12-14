#ifndef SNAKE_FRUIT_HPP
#define SNAKE_FRUIT_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace sf;
using namespace std;

class Fruit {
private:
    RectangleShape sprite;
    const Texture* texture = nullptr;
    int frameCount = 3;
    int currentFrame = 0;

public:
    Fruit();

    void setPosition(Vector2f position);
    void setTexture(const Texture* tex, int frames = 3);
    void pickRandomFrame();

    const RectangleShape& getSprite() const;

};

#endif //SNAKE_FRUIT_HPP