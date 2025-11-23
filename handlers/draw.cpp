#include "engine.hpp"

void Engine::draw(){
    window.clear(Color::Black);

    if (currentGameState != GameState::MENU) {
        // Map
        map.Draw(window);

        // Fruit
        window.draw(fruit.getSprite());

        // Snake
        for(auto & s : snake){
            window.draw(s.getShape());
        }

        drawDirectionArrow();
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

    sf::Image img;

    if (!img.loadFromFile(RESOURCE_DIR "/levels/Level01.png")) {
        // fallback
        return;
    }

    map.CreateFromImage(img);
}