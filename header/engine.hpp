#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include "snake.hpp"
#include "fruit.hpp"
#include "map.hpp"
#include "resource_path.h"
#include "mainMenuScreen.hpp"
#include "pauseScreen.hpp"
#include "gameOverScreen.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <optional>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 720;

static const float VIEW_HEIGHT = 900.f;

class Engine {
    private:
        // Window
        RenderWindow window;
        const unsigned int FPS = 120;
        static const Time timePerFrame;

        // Map
        Map map{20.0f};
        Texture wallText;

        // View
        View view;
        View uiView;

        // Snake
        vector<Snake> snake;
        int snakeDirection;
        deque<int> direction;
        int speed;
        int sectionToAdd; 
        Texture snakeSkin;

        // Fruit
        Fruit fruit;
        Texture fruitAtlas;

        // Arrow
        Texture arrowAtlas;
        RectangleShape arrowSprite;
        int currentArrowFrame;
        Time arrowAnimationTimer;
        const float ARROW_ANIMATION_SPEED = 0.5f;
        const float ARROW_MARGIN = 50.0f;
        const int ARROW_FRAME_COUNT = 3;

        Time timeSinceLastMove;

        int currentGameState; 
        int lastGameState;

        // Score system
        int score;
        Font uiFont;
        std::optional<Text> scoreText;
        bool scoreFontLoaded;

        MainMenuScreen mainMenuScreen;
        PauseScreen pauseScreen; 
        GameOverScreen gameOverScreen;

        void setCurrentView(float dtSeconds);
        void ResizeView(RenderWindow& window);
        void buildMapFromLevelImage();
        void updateScoreText();

    public:
        enum Direction { UP, RIGHT, DOWN, LEFT };
        enum GameState { MENU, RUNNING, PAUSED, GAMEOVER };

        Engine();

        RenderWindow& getWindow() { return window; }
        const View& getUIView() const { return uiView; }

        GameState getGameState() const { return (GameState)currentGameState; }
        void setGameState(GameState state) { currentGameState = state; }

        int getScore() const { return score; }
        void resetScore();
        void addScore(int delta);

        void handleGameKeyPressed(const Event::KeyPressed& e);

        void startGame();

        void input();
        void addDirection(int newDirection);
        void togglePause();

        void update();

        void draw();
        
        void moveFruit();

        void run();

        void newSnake();
        void addSnakeSection();

        void setupDirectionArrow();
        void updateDirectionArrow();
        Vector2f calculateArrowPosition(Vector2f snakePos, Vector2f fruitPos);
        float calculateArrowRotation(Vector2f snakePos, Vector2f fruitPos);
        void drawDirectionArrow();

};

#endif //SNAKE_ENGINE_HPP