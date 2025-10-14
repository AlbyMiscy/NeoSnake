#include "engine.hpp"

void Engine::input(){
    window.handleEvents(
            [this](const sf::Event::Closed&){
                window.close();
            },
            [this](const sf::Event::KeyPressed& e) {
                switch (e.scancode)
                {
                case Keyboard::Scancode::Escape:
                    window.close();
                    return;
                case Keyboard::Scancode::Up:
                    addDirection(Direction::UP);
                    break; 
                case Keyboard::Scancode::Down:
                    addDirection(Direction::DOWN);
                    break; 
                case Keyboard::Scancode::Left:
                    addDirection(Direction::LEFT);
                    break; 
                case Keyboard::Scancode::Right:
                    addDirection(Direction::RIGHT);
                    break; 
                default:
                    break;
                }
            }
        );
}

void Engine::addDirection(int newDirection){
    if(direction.empty()){
        direction.emplace_back(newDirection);
    }
    else{
        if(direction.back() != newDirection){
            direction.emplace_back(newDirection);
        }
    }
}