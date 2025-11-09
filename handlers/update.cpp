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
                if(direction.front() != Direction::DOWN)  snakeDirection = direction.front();
                break;
            case Direction::DOWN:
                if(direction.front() != Direction::UP)    snakeDirection = direction.front();
                break;
            case Direction::LEFT:
                if(direction.front() != Direction::RIGHT) snakeDirection = direction.front();
                break;
            case Direction::RIGHT:
                if(direction.front() != Direction::LEFT)  snakeDirection = direction.front();
                break;
            default: break;
            }
            direction.pop_front();
        }

        if(sectionToAdd){
            addSnakeSection();
            sectionToAdd--;
        }

        // Collision detection - Fruit

        // Compute the movement offset of the snake's head based on the current direction
        const float cell = map.getCellSize();

        // Compute the next head position after the move
        Vector2f step(0.f, 0.f);
        switch (snakeDirection)
        {
        case Direction::RIGHT: step.x =  cell; break;
        case Direction::DOWN:  step.y =  cell; break;
        case Direction::LEFT:  step.x = -cell; break;
        case Direction::UP:    step.y = -cell; break;
        default: break;
        }

        // Compute the next head position after the move
        Vector2f nextHead = thisSectionPosition + step;

        // Get total map dimensions in world coordinates
        const float mapW = map.getWorldWidth();
        const float mapH = map.getWorldHeight();

        // Check if the next position is outside the map boundaries
        if (nextHead.x < 0.f || nextHead.y < 0.f ||
            nextHead.x + cell > mapW || nextHead.y + cell > mapH)
        {
            currentGameState = GameState::GAMEOVER;
            timeSinceLastMove = Time::Zero;
            return;
        }

        // Convert the next head position into map grid coordinates
        Vector2i c = map.worldToCell(nextHead);

        // Check if the destination cell corresponds to a wall in the map
        if (map.isWallCell(static_cast<size_t>(c.x), static_cast<size_t>(c.y))) {
            currentGameState = GameState::GAMEOVER;
            timeSinceLastMove = Time::Zero;
            return;
        }

        // Update the snake's head position
        switch (snakeDirection)
        {
        case Direction::RIGHT:
            snake[0].setPosition(Vector2f(thisSectionPosition.x + cell, thisSectionPosition.y));
            break;
        case Direction::DOWN:
            snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y + cell));
            break;
        case Direction::LEFT:
            snake[0].setPosition(Vector2f(thisSectionPosition.x - cell, thisSectionPosition.y));
            break;
        case Direction::UP:
            snake[0].setPosition(Vector2f(thisSectionPosition.x, thisSectionPosition.y - cell));
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
                timeSinceLastMove = Time::Zero;
                return;
            }
        }

        // Reset the last move timer
        timeSinceLastMove = Time::Zero;
    } 
}