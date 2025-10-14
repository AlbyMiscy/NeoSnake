#include "engine.hpp"

void Engine::moveFruit(){
    // find a location to place the fruit
    // must nt be inside the snake, or the wall

    sf::Vector2f fruitResolution(
        static_cast<float>(WINDOW_WIDTH) / 20.f - 2.f,
        static_cast<float>(WINDOW_HEIGHT) / 20.f - 2.f
    );
    Vector2f newFruitLocation;
    bool badLocation = false;
    srand(static_cast<unsigned int>(time(nullptr)));
    do {
        badLocation = false;
        newFruitLocation.x = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)fruitResolution.x)) * 20;
        newFruitLocation.y = (float)(1 + rand() / ((RAND_MAX + 1u) / (int)fruitResolution.y)) * 20;

        // Check if it is in the snake
        for(auto & s : snake){
            const sf::FloatRect snakeBounds = s.getShape().getGlobalBounds();
            const sf::FloatRect fruitRect({newFruitLocation.x, newFruitLocation.y},
                                        {20.f, 20.f});
        
            if(snakeBounds.findIntersection(fruitRect)){
                badLocation = true;
                break;
            }
        }
    }
    while (badLocation);
    fruit.setPosition(newFruitLocation);
}