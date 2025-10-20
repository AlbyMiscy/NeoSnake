#include "engine.hpp"

void Engine::update(){
    // update snake position
    if(timeSinceLastMove.asSeconds() >= seconds(1.f/float(speed)).asSeconds()){
        Vector2f thisSectionPosition = snake[0].getPosition();
        Vector2f lastSectionPosition = thisSectionPosition;

        if(!direction.empty()){
            switch (snakeDirection)
            {
            case Direction::UP:
                if(direction.front() != Direction::DOWN){
                    snakeDirection = direction.front();
                }
                break;
            case Direction::DOWN:
                if(direction.front() != Direction::UP){
                    snakeDirection = direction.front();
                }
                break;
            case Direction::LEFT:
                if(direction.front() != Direction::RIGHT){
                    snakeDirection = direction.front();
                }
                break;
            case Direction::RIGHT:
                if(direction.front() != Direction::LEFT){
                    snakeDirection = direction.front();
                }
                break;
            
            default:
                break;
            }
            direction.pop_front();
        }

        if(sectionToAdd){
            addSnakeSection();
            sectionToAdd--;
        }

        // Update the snake's head position
        switch (snakeDirection)
        {
        case Direction::RIGHT:
            snake[0].setPosition(Vector2f(thisSectionPosition.x + 20, thisSectionPosition.y));
            break;
        case Direction::DOWN:
            snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y + 20));
            break;
        case Direction::LEFT:
            snake[0].setPosition(Vector2f(thisSectionPosition.x - 20, thisSectionPosition.y));
            break;
        case Direction::UP:
            snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y - 20));
            break;
        
        default:
            break;
        }

        // Update the snake's tail position
        for(size_t s = 1; s < snake.size(); s++){
            thisSectionPosition = snake[s].getPosition();
            snake[s].setPosition(lastSectionPosition);
            lastSectionPosition = thisSectionPosition;
        }

        // run snake section update function
        for(auto & s : snake){
            s.update();
        }

        // Collision detection - Fruit
        if(snake[0].getShape().getGlobalBounds().findIntersection(fruit.getSprite().getGlobalBounds())){
            sectionToAdd += 4;
            speed++;
            moveFruit();
        }

        // Collision detection - Snake Body
        for(size_t s = 1; s < snake.size(); s++){
            if(snake[0].getShape().getGlobalBounds().findIntersection(snake[s].getShape().getGlobalBounds())){
                currentGameState = GAMEOVER;
            }
        }

        // Reset the last move timer
        timeSinceLastMove = Time::Zero;
    } // END update snake position
}