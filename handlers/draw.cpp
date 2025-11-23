#include "engine.hpp"

void Engine::draw(){
    window.clear(Color::Black);

    // Map
    map.Draw(window);

    // Draw fruit section
    window.draw(fruit.getSprite());

    // Draw snake section
    for(auto & s : snake){
        window.draw(s.getShape());
    }
    
    // Draw direction arrow
    drawDirectionArrow();

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