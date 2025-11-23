#include "engine.hpp"

const Time Engine::timePerFrame = seconds(1.f/60.f);

Engine::Engine(){
    window.create(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "NeoSnake", Style::Resize);
    window.setFramerateLimit(FPS);

    currentGameState = GameState::MENU;
    lastGameState = currentGameState;
}

void Engine::startGame(){
    speed = 4;
    snakeDirection = Direction::RIGHT;
    
    timeSinceLastMove = Time::Zero;
    sectionToAdd = 0;
    
    if(fruitAtlas.loadFromFile(RESOURCE_DIR "/texture/fruit.png"))
        fruit.setTexture(&fruitAtlas, 3);

    // Load snake skin
    if(!snakeSkin.loadFromFile(RESOURCE_DIR "/texture/skin/snake_skin_1-1.png")){
        // If loading fails, continue - snake will use fallback fill color
    }

    if(arrowAtlas.loadFromFile(RESOURCE_DIR "/texture/arrow.png"))
        setupDirectionArrow();

    newSnake();
    moveFruit();

    float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
    view.setSize({VIEW_HEIGHT * aspect, VIEW_HEIGHT});
    setCurrentView(0.0f);

    buildMapFromLevelImage();

    direction.clear();

    currentGameState = GameState::RUNNING;
    lastGameState = currentGameState;
}

void Engine::run(){
    Clock clock;

    while(window.isOpen()){
        Time dt = clock.restart();

        input();  

        if (currentGameState == GameState::RUNNING) {
            timeSinceLastMove += dt;
            setCurrentView(dt.asSeconds());
            update();
        }

        draw();

        sleep(milliseconds(2));
    }
}


void Engine::setCurrentView(float dtSeconds) {
    Vector2f target = snake.front().getPosition();

    Vector2f halfView = view.getSize() / 2.f;

    float mapW = map.getWorldWidth();
    float mapH = map.getWorldHeight();

    target.x = clamp(target.x, halfView.x, mapW - halfView.x);
    target.y = clamp(target.y, halfView.y, mapH - halfView.y);

    Vector2f current = view.getCenter();

    const float cameraSpeed = 8.0f;

    Vector2f direction = target - current;
    Vector2f newCenter = current + direction * (cameraSpeed * dtSeconds);

    view.setCenter(newCenter);
    window.setView(view);
}