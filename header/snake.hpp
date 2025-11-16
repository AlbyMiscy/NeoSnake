#ifndef SNAKE_SNAKE_HPP
#define SNAKE_SNAKE_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

class Snake{
private:
    Vector2f position;
    RectangleShape section;

public:
    Snake(Vector2f startPosition);

    Vector2f getPosition();
    void setPosition(Vector2f);

    RectangleShape& getShape();

    // Texture / appearance helpers
    void setTexture(sf::Texture* texture);
    void setTextureRect(const sf::IntRect& rect);
    void setRotation(float angle);

    void update();
};

#endif //SNAKE_SNAKE_HPP