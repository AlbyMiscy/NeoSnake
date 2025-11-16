#include "engine.hpp"

void Engine::newSnake() {
    snake.clear();
    // initial three sections
    snake.emplace_back(Vector2f(100,100));
    snake.emplace_back(Vector2f(80,100));
    snake.emplace_back(Vector2f(60,100));

    // Apply skin texture and initial rect/rotation
    const sf::IntRect headRect({0, 0}, {20, 20});
    const sf::IntRect bodyRect({20, 0}, {20, 20});

    for (size_t i = 0; i < snake.size(); ++i) {
        snake[i].setTexture(&snakeSkin);
        if (i == 0) {
            snake[i].setTextureRect(headRect);
            // initial direction is RIGHT by default in Engine::startGame
            snake[i].setRotation(90.f);
        } else {
            snake[i].setTextureRect(bodyRect);
            // body initially laid out horizontally to the left of head -> pointing right (toward head)
            snake[i].setRotation(90.f);
        }
    }
}

void Engine::addSnakeSection (){
    Vector2f newSectionPosition = snake[snake.size() - 1].getPosition();
    snake.emplace_back(newSectionPosition);
    // When adding a section, apply texture and body rect. Rotation will be corrected on next update.
    const sf::IntRect bodyRect({20, 0}, {20, 20});
    snake.back().setTexture(&snakeSkin);
    snake.back().setTextureRect(bodyRect);
    // default rotation vertical (0)
    snake.back().setRotation(0.f);
}