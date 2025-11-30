#include "engine.hpp"

void Engine::draw(){
    window.clear(Color::Black);

    // Assicura che la view del gioco sia attiva per disegnare il mondo
    window.setView(view);

    if (currentGameState != GameState::MENU) {
        // Map
        map.Draw(window);

        // Fruit
        window.draw(fruit.getSprite());

        // Snake
        for(auto & s : snake){
            window.draw(s.getShape());
        }
        
        // Enemies
        drawEnemies();

        drawDirectionArrow();
    }

    // Draw score HUD when in RUNNING state
    if (currentGameState == GameState::RUNNING && scoreFontLoaded && scoreText) {
        View oldView = window.getView();
        window.setView(uiView);
        window.draw(*scoreText);
        window.setView(oldView);
    }

    switch (currentGameState)
    {
        case GameState::MENU:
            mainMenuScreen.draw(*this);
            break;
        case GameState::PAUSED:
            pauseScreen.draw(*this);
            break;
        case GameState::GAMEOVER:
            gameOverScreen.draw(*this);
            break;
        case GameState::RUNNING:
        default:
            break;
    }

    window.display();
}


void Engine::buildMapFromLevelImage(){
    if(!wallText.loadFromFile(RESOURCE_DIR "/texture/wall.png")){
        return;
    }
    else map.SetTileTexture(&wallText);

    Image img;

    if (!img.loadFromFile(RESOURCE_DIR "/levels/Level01.png")) {
        // fallback
        return;
    }

    map.CreateFromImage(img);
    
    mapSize = img.getSize();
    
    vector<Vector2u> patrolA; // giallo
    vector<Vector2u> patrolB; // verde
    
    for (unsigned int x = 0; x < img.getSize().x; ++x) {
        for (unsigned int y = 0; y < img.getSize().y; ++y) {
            Color pixel = img.getPixel({x, y});
            
            // Giallo = patrol point A
            if (pixel == Color::Yellow) {
                patrolA.emplace_back(x, y);
            }
            // Verde = patrol point B
            else if (pixel == Color::Green) {
                patrolB.emplace_back(x, y);
            }
        }
    }
    
    enemies.clear();
    size_t n = std::min(patrolA.size(), patrolB.size());
    
    // Classe helper per GridProvider
    class MyGrid : public GridProvider {
        const Engine* engine;
    public:
        MyGrid(const Engine* e) : engine(e) {}
        
        Vector2u size() const override {
            return engine->mapSize;
        }
        
        bool isBlocked(Vector2u tile) const override {
            return engine->isBlocked(tile);
        }
    };
    
    MyGrid grid(this);
    
    for (size_t i = 0; i < n; ++i) {
        Enemy::TextureType type = (i % 2 == 0) ? Enemy::Enemy1 : Enemy::Enemy2;
        
        enemies.emplace_back(patrolA[i], patrolB[i], type);
        
        deque<Vector2u> path = astar(grid, patrolA[i], patrolB[i]);
        
        if (!path.empty()) {
            enemies.back().setPath(path);
        }
    }
}

void Engine::drawEnemies() {
    for (const auto& enemy : enemies) {
        enemy.draw(window);
    }
}
