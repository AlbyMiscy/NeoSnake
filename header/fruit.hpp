#ifndef SNAKE_FRUIT_HPP
#define SNAKE_FRUIT_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class Fruit {
private:
    RectangleShape sprite;

public:
    Fruit();

    void setPosition(Vector2f position);
    RectangleShape getSprite();

};

#endif //SNAKE_FRUIT_HPP