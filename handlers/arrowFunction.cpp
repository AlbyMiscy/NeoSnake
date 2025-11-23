#include "engine.hpp"

void Engine::setupDirectionArrow(){
    currentArrowFrame = 0; 
    arrowAnimationTimer = Time::Zero;
    
    // Setup arrow sprite (larger than fruit)
    arrowSprite.setSize(Vector2f(40.f, 40.f));
    arrowSprite.setTexture(&arrowAtlas, true);
    arrowSprite.setFillColor(Color::White);
    
    // Set initial texture rect (first frame)
    const auto tw = arrowAtlas.getSize().x;
    const auto th = arrowAtlas.getSize().y;
    const unsigned frameW = tw / ARROW_FRAME_COUNT;
    
    arrowSprite.setTextureRect(IntRect(Vector2i(0, 0), Vector2i(static_cast<int>(frameW), static_cast<int>(th))));
    
    // Set origin to center for rotation
    arrowSprite.setOrigin(Vector2f(20.f, 20.f));
}

void Engine::updateDirectionArrow(){
    // Only update if we're in the running state
    if (currentGameState != GameState::RUNNING) {
        return;
    }
    
    // Update animation timer
    arrowAnimationTimer += timePerFrame;
    
    // Update animation frame
    if (arrowAnimationTimer.asSeconds() >= ARROW_ANIMATION_SPEED) {
        currentArrowFrame = (currentArrowFrame + 1) % ARROW_FRAME_COUNT; // Cycle through frames
        
        const auto tw = arrowAtlas.getSize().x;
        const auto th = arrowAtlas.getSize().y;
        const unsigned frameW = tw / ARROW_FRAME_COUNT;
        const int left = currentArrowFrame * static_cast<int>(frameW);
        
        arrowSprite.setTextureRect(IntRect(Vector2i(left, 0), Vector2i(static_cast<int>(frameW), static_cast<int>(th))));
        arrowAnimationTimer = Time::Zero;
    }
    
    // Get snake head and fruit positions
    Vector2f snakePos = snake[0].getPosition();
    Vector2f fruitPos = fruit.getSprite().getPosition();
    
    // Calculate arrow position and rotation
    Vector2f arrowPos = calculateArrowPosition(snakePos, fruitPos);
    float arrowRotation = calculateArrowRotation(snakePos, fruitPos);
    
    // Apply position and rotation to sprite
    arrowSprite.setPosition(arrowPos);
    arrowSprite.setRotation(degrees(arrowRotation));
}

Vector2f Engine::calculateArrowPosition(Vector2f snakePos, Vector2f fruitPos) {
    Vector2f cameraCenter = view.getCenter();
    Vector2f camSize = view.getSize();
    float cameraHalfWidth = camSize.x / 2.0f;
    float cameraHalfHeight = camSize.y / 2.0f;
    
    // Calculate direction from snake to fruit
    Vector2f direction = fruitPos - snakePos;
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length == 0) {
        return cameraCenter; 
    }
    
    direction /= length;
    
    Vector2f arrowPos = cameraCenter;
    
    float scaleX = (cameraHalfWidth - ARROW_MARGIN) / abs(direction.x);
    float scaleY = (cameraHalfHeight - ARROW_MARGIN) / abs(direction.y);
    
    float scale = min(scaleX, scaleY);
    
    arrowPos += direction * scale;
    
    return arrowPos;
}

float Engine::calculateArrowRotation(Vector2f snakePos, Vector2f fruitPos) {
    Vector2f direction = fruitPos - snakePos;
    float angle = atan2(direction.y, direction.x);
    
    // Convert radians to degrees
    float degrees = angle * 180.0f / 3.14159f;
    
    return degrees;
}

void Engine::drawDirectionArrow() {
    // Only draw if we're in the running state
    if (currentGameState != GameState::RUNNING) {
        return;
    }
    
    // Check if fruit is outside camera view
    Vector2f cameraCenter = view.getCenter();
    Vector2f fruitPos = fruit.getSprite().getPosition();
    
    Vector2f camSize = view.getSize();
    float cameraLeft = cameraCenter.x - camSize.x / 2.0f;
    float cameraRight = cameraCenter.x + camSize.x / 2.0f;
    float cameraTop = cameraCenter.y - camSize.y / 2.0f;
    float cameraBottom = cameraCenter.y + camSize.y / 2.0f;
    
    // Only draw arrow if fruit is outside camera view
    bool fruitOutsideView = (fruitPos.x < cameraLeft || fruitPos.x > cameraRight ||
                            fruitPos.y < cameraTop || fruitPos.y > cameraBottom);
    
    if (fruitOutsideView) {
        window.draw(arrowSprite);
    }
}