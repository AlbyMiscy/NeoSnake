#include "engine.hpp"

const Time Engine::timePerFrame = seconds(1.f/60.f);

Engine::Engine(){
    window.create(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "NeoSnake", Style::Default);
    window.setFramerateLimit(FPS);

    speed = 4;
    snakeDirection = Direction::RIGHT;

    timeSinceLastMove = Time::Zero;
    sectionToAdd = 0;

    newSnake();
    moveFruit();
}

void Engine::run(){
    Clock clock;

    // main loop - Runs until the window is closed
    while(window.isOpen()){
        Time dt = clock.restart();
        timeSinceLastMove += dt;

        input();
        update();
        draw();
    }
}