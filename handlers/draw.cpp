#include "engine.hpp"

void Engine::draw(){
    window.clear(Color::Black);

    // Assicura che la view del gioco sia attiva per disegnare il mondo
    window.setView(view);

    if (currentGameState != GameState::MENU) {
        // Map
        levelMap.draw(window);

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
        case GameState::NEXTLEVEL:
            nextLevelScreen.draw(*this);
            break;
        case GameState::RUNNING:
        default:
            break;
    }

    window.display();
}


void Engine::loadLevel(int levelIndex){
    // Carica il tileset una sola volta
    if(!tilesetTexture.loadFromFile(RESOURCE_DIR "/texture/map_tileset.png")){
        cerr << "Errore: impossibile caricare map_tileset.png" << endl;
        return;
    }

    // Costruisci il percorso della cartella del livello
    ostringstream oss;
    oss << RESOURCE_DIR "/maps/level" << setfill('0') << setw(2) << levelIndex;
    string levelFolderPath = oss.str();

    // Carica la mappa dal livello
    if (!levelMap.loadFromFolder(levelFolderPath, tilesetTexture)) {
        cerr << "Errore: impossibile caricare il livello " << levelIndex << endl;
        return;
    }

    // Aggiorna mapSize in base alla griglia caricata
    mapSize = levelMap.getGridSize();

    // Ottieni i punti di pattuglia
    const vector<Vector2u>& patrolA = levelMap.getPatrolPointsA();
    const vector<Vector2u>& patrolB = levelMap.getPatrolPointsB();

    enemies.clear();

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

    // Per ogni punto GIALLO (patrol start):
    // 1) Trova il punto VERDE più vicino usando findNearestGreenPatrol
    // 2) Calcola il percorso con A* UNA SOLA VOLTA
    // 3) Crea il nemico e assegna il percorso
    for (size_t i = 0; i < patrolA.size(); ++i) {
        Vector2u yellowStart = patrolA[i];
        
        // Trova il punto verde più vicino a questo punto giallo
        Vector2u greenEnd = findNearestGreenPatrol(yellowStart, patrolB);
        
        // Alterna il tipo di texture per variare l'aspetto dei nemici
        Enemy::TextureType type = (i % 2 == 0) ? Enemy::Enemy1 : Enemy::Enemy2;

        // Crea il nemico (i parametri yellowStart e greenEnd sono usati solo per inizializzare la posizione)
        enemies.emplace_back(yellowStart, greenEnd, type);

        // Calcola il percorso con A* dal punto giallo al punto verde associato
        deque<Vector2u> path = astar(grid, yellowStart, greenEnd);

        // Assegna il percorso al nemico (verrà seguito in loop con reverse)
        if (!path.empty()) {
            enemies.back().setPath(path);
        } else {
            // Se A* non trova un percorso, segnala l'errore
            cerr << "Attenzione: nessun percorso trovato per il nemico " << i 
                 << " da (" << yellowStart.x << "," << yellowStart.y << ") a (" 
                 << greenEnd.x << "," << greenEnd.y << ")" << endl;
        }
    }
}

void Engine::drawEnemies() {
    for (const auto& enemy : enemies) {
        enemy.draw(window);
    }
}
