#include "engine.hpp"

void Engine::draw(){
    window.clear(Color::Black);

    // Draw fruit section
    window.draw(fruit.getSprite());

    // Draw snake section
    for(auto & s : snake){
        window.draw(s.getShape());
    }

    window.display();
}