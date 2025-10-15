#include "engine.hpp"

const Time Engine::timePerFrame = seconds(1.f/60.f);

Engine::Engine(){
    window.create(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "NeoSnake", Style::Resize);
    window.setFramerateLimit(FPS);
    
    speed = 4;
    snakeDirection = Direction::RIGHT;
    
    timeSinceLastMove = Time::Zero;
    sectionToAdd = 0;
    
    newSnake();
    moveFruit();

    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    view.setSize({VIEW_HEIGHT * aspect, VIEW_HEIGHT});
    setCurrentView();
}

void Engine::run(){
    Clock clock;

    // main loop - Runs until the window is closed
    while(window.isOpen()){
        Time dt = clock.restart();
        timeSinceLastMove += dt;
        setCurrentView();
        input();
        update();
        draw();
    }
}

void Engine::setCurrentView(){
    view.setCenter(snake.front().getPosition());
    window.setView(view);
}