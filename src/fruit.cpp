#include "fruit.hpp"

Fruit::Fruit(){
    Vector2f startPosition(400, 300);
    sprite.setSize(Vector2f(20,20));
    sprite.setFillColor(Color::Red);
    sprite.setPosition(startPosition);
}

void Fruit::setPosition(Vector2f newPosition){
    sprite.setPosition(newPosition);
}

RectangleShape Fruit::getSprite(){
    return sprite;
}