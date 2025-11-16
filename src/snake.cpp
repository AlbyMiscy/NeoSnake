#include "snake.hpp"

Snake::Snake(Vector2f startPosition){
    section.setSize(Vector2f(20,20));
    section.setFillColor(Color::Green);
    // Set origin to center so rotations happen around the visual center of the tile
    section.setOrigin(Vector2f(10.f, 10.f));
    section.setPosition(startPosition + Vector2f(10.f, 10.f));
    position = startPosition;
}

Vector2f Snake::getPosition(){
    return position;
}

void Snake::setPosition(Vector2f newPosition){
    position = newPosition;
}

RectangleShape& Snake::getShape(){
    return section;
}

void Snake::setTexture(sf::Texture* texture){
    section.setTexture(texture);
}

void Snake::setTextureRect(const sf::IntRect& rect){
    section.setTextureRect(rect);
}

void Snake::setRotation(float angle){
    section.setRotation(sf::degrees(angle));
}

void Snake::update(){
    // Position stored in `position` is the top-left of the cell; store the shape at the cell center
    section.setPosition(position + Vector2f(10.f, 10.f));
}