#include "engine.hpp"

void Engine::input(){
    window.handleEvents(
        [this](const sf::Event::Closed&){
            window.close();
        },
        [this](const sf::Event::Resized&){
            ResizeView(window);
        },
        [this](const sf::Event::KeyPressed& e) {

            switch (currentGameState)
            {
                case GameState::MENU:
                    mainMenuScreen.handleKeyPressed(*this, e);
                    break;

                case GameState::RUNNING:
                    handleGameKeyPressed(e);
                    break;

                case GameState::PAUSED:
                    pauseScreen.handleKeyPressed(*this, e);
                    break;

                case GameState::GAMEOVER:
                    gameOverScreen.handleKeyPressed(*this, e);
                    break;

                case GameState::NEXTLEVEL:
                    nextLevelScreen.handleKeyPressed(*this, e);
                    break;
            }
        }
    );
}

void Engine::handleGameKeyPressed(const Event::KeyPressed& e){
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

void Engine::ResizeView(RenderWindow& window){
    auto size = window.getSize();
    float aspectRatio = float(size.x) / float(size.y);

    view.setSize(Vector2f(VIEW_HEIGHT * aspectRatio, VIEW_HEIGHT));
    window.setView(view);

    uiView.setSize(Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    uiView.setCenter(uiView.getSize() / 2.f);
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