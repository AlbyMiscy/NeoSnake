#include "engine.hpp"

const Time Engine::timePerFrame = seconds(1.f/60.f);

Engine::Engine(){
    window.create(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "NeoSnake", Style::Resize);
    window.setFramerateLimit(FPS);
<<<<<<< HEAD
    
=======
    startGame();
}

void Engine::startGame(){
>>>>>>> 9eeeb6661795f42b6c46bf3acb0ee92e3d572a0e
    speed = 4;
    snakeDirection = Direction::RIGHT;
    
    timeSinceLastMove = Time::Zero;
    sectionToAdd = 0;
<<<<<<< HEAD
    
    newSnake();
    moveFruit();

    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    view.setSize({VIEW_HEIGHT * aspect, VIEW_HEIGHT});
    setCurrentView();
=======
    direction.clear();

    newSnake();
    moveFruit();

    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
>>>>>>> 9eeeb6661795f42b6c46bf3acb0ee92e3d572a0e
}

void Engine::run(){
    Clock clock;

    // main loop - Runs until the window is closed
    while(window.isOpen()){
        Time dt = clock.restart();

        if(currentGameState == GameState::PAUSED || currentGameState == GameState::GAMEOVER){
            input();
            if(currentGameState == GameState::GAMEOVER){
                draw();
            }
            sleep(milliseconds(2));
            continue;
        }

        timeSinceLastMove += dt;
        setCurrentView();
        input();
        update();
        draw();
    }
}

void Engine::setCurrentView() {
    view.setCenter(snake.front().getPosition());
    window.setView(view);
}