#include "snake.hpp"

Snake::Snake(Vector2f startPosition){
    section.setSize(Vector2f(20,20));
    section.setFillColor(Color::Green);
    section.setPosition(startPosition);
    position = startPosition;
}

Vector2f Snake::getPosition(){
    return position;
}

void Snake::setPosition(Vector2f newPosition){
    position = newPosition;
}

RectangleShape Snake::getShape(){
    return section;
}

void Snake::update(){
    section.setPosition(position);
}