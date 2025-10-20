#include "engine.hpp"

const Time Engine::timePerFrame = seconds(1.f/60.f);

Engine::Engine(){
    window.create(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "NeoSnake", Style::Default);
    window.setFramerateLimit(FPS);
    startGame();
}

void Engine::startGame(){
    speed = 4;
    snakeDirection = Direction::RIGHT;

    timeSinceLastMove = Time::Zero;
    sectionToAdd = 0;
    direction.clear();

    newSnake();
    moveFruit();

    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
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

        input();
        update();
        draw();
    }
}