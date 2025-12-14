#include "engine.hpp"

void Engine::update(){
    if(currentGameState != GameState::RUNNING)
        return;

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
        const float cell = levelMap.getCellSize();

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
        const float mapW = levelMap.getWorldWidth();
        const float mapH = levelMap.getWorldHeight();

        // Check if the next position is outside the map boundaries
        if (nextHead.x < 0.f || nextHead.y < 0.f ||
            nextHead.x + cell > mapW || nextHead.y + cell > mapH)
        {
            currentGameState = GameState::GAMEOVER;
            gameOverScreen.setScore(score);
            timeSinceLastMove = Time::Zero;
            return;
        }

        // Convert the next head position into map grid coordinates
        Vector2i c = levelMap.worldToCell(nextHead);

        // Check if the destination cell corresponds to a wall in the map
        if (levelMap.isWallCell(static_cast<size_t>(c.x), static_cast<size_t>(c.y))) {
            currentGameState = GameState::GAMEOVER;
            gameOverScreen.setScore(score);
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

        // Apply snake skin frames and rotations 
        // Texture frames: head = (0,0,20,20), body = (20,0,20,20)
        const IntRect headRect({0, 0}, {20, 20});
        const IntRect bodyRect({20, 0}, {20, 20});

        // Head: set texture rect and rotation according to current snakeDirection
        if (!snake.empty()) {
            snake[0].setTexture(&snakeSkin);
            snake[0].setTextureRect(headRect);
            float headAngle = 0.f;
            switch (snakeDirection) {
                case Direction::UP: headAngle = 0.f; break;
                case Direction::RIGHT: headAngle = 90.f; break;
                case Direction::DOWN: headAngle = 180.f; break;
                case Direction::LEFT: headAngle = 270.f; break;
                default: break;
            }
            snake[0].setRotation(headAngle);
        }

        // Body: for each body segment, orient it towards the segment ahead (index-1)
        for (size_t s = 1; s < snake.size(); ++s) {
            snake[s].setTexture(&snakeSkin);
            snake[s].setTextureRect(bodyRect);

            Vector2f cur = snake[s].getPosition();
            Vector2f ahead = snake[s-1].getPosition();
            Vector2f delta = ahead - cur;

            float angle = 0.f;
            if (abs(delta.x) > abs(delta.y)) {
                // horizontal
                if (delta.x > 0.f) angle = 90.f; // facing right
                else angle = 270.f; // facing left
            } else {
                // vertical
                if (delta.y > 0.f) angle = 180.f; // facing down
                else angle = 0.f; // facing up
            }
            snake[s].setRotation(angle);
        }

        // Collision detection - Fruit
        if(snake[0].getShape().getGlobalBounds().findIntersection(fruit.getSprite().getGlobalBounds())){
            sectionToAdd += 4;
            speed++;
            addScore(1);
            fruitsEaten++;
            
            // Controlla se bisogna passare al livello successivo
            bool shouldLevelUp = false;
            for (size_t i = 0; i < fruitThresholds.size(); ++i) {
                if (i == fruitThresholds.size() - 1) {
                    // Ultimo threshold raggiunto, nessun altro livello
                    break;
                }
                if (fruitsEaten == fruitThresholds[i + 1] && currentLevel == static_cast<int>(i + 1)) {
                    shouldLevelUp = true;
                    break;
                }
            }
            
            if (shouldLevelUp && currentLevel < static_cast<int>(fruitThresholds.size())) {
                // Mostra la schermata di passaggio livello
                currentGameState = GameState::NEXTLEVEL;
                timeSinceLastMove = Time::Zero;
                return;
            }
            
            moveFruit(); 
        }

        // Collision detection - Snake Body
        for(size_t s = 1; s < snake.size(); s++){
            if(snake[0].getShape().getGlobalBounds().findIntersection(snake[s].getShape().getGlobalBounds())){
                currentGameState = GAMEOVER;
                gameOverScreen.setScore(score);
                timeSinceLastMove = Time::Zero;
                return;
            }
        }

        // Reset the last move timer
        timeSinceLastMove = Time::Zero;
    }
    
    // Update enemies con il frame delta time
    updateEnemies(deltaTime.asSeconds());
    
    // Update direction arrow animation
    updateDirectionArrow();
}

void Engine::updateEnemies(float dt) {
    for (auto& enemy : enemies) {
        enemy.update(dt);
        
        if (enemy.atEnd()) {
            enemy.reverse();
        }
    }
    
    if (!snake.empty()) {
        FloatRect snakeHeadBounds = snake.front().getShape().getGlobalBounds();
        
        for (const auto& enemy : enemies) {
            if (snakeHeadBounds.findIntersection(enemy.bounds())) {
                currentGameState = GameState::GAMEOVER;
                return;
            }
        }
    }
}