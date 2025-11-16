#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include "snake.hpp"
#include "fruit.hpp"
#include "map.hpp"
#include "resource_path.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>

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

        // Snake
        vector<Snake> snake;
        int snakeDirection;
        deque<int> direction;
        int speed;
        int sectionToAdd; 

        // Fruit
        Fruit fruit;
        Texture fruitAtlas;
        Texture snakeSkin;

        Time timeSinceLastMove;

        int currentGameState; 
        int lastGameState;

        void setCurrentView(float dtSeconds);
        void ResizeView(const RenderWindow& window, View& view);
        void buildMapFromLevelImage();

    public:
        enum Direction { UP, RIGHT, DOWN, LEFT };
        enum GameState { RUNNING, PAUSED, GAMEOVER };

        Engine();

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

};

#endif //SNAKE_ENGINE_HPP