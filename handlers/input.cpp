#include "engine.hpp"

void Engine::input(){
    window.handleEvents(
            [this](const sf::Event::Closed&){
                window.close();
            },
            [this](const sf::Event::Resized&){
                ResizeView(window, view);
            },
            [this](const sf::Event::KeyPressed& e) {
                if(currentGameState == GameState::GAMEOVER){
                    if(e.scancode == Keyboard::Scancode::Enter){
                        startGame();
                    }
                }
                switch (e.scancode)
                {
                case Keyboard::Scancode::P:
                    togglePause();
                    break;
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

void Engine::ResizeView(const RenderWindow& window, View& view){
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(Vector2f(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT));
}

void Engine::togglePause(){
    if(currentGameState == GameState::RUNNING){
        lastGameState = currentGameState;
        currentGameState = GameState::PAUSED;
    }
    else if(currentGameState == GameState::PAUSED){
        currentGameState = lastGameState;
    }
}