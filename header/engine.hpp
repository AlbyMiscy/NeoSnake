#ifndef SNAKE_ENGINE_HPP
#define SNAKE_ENGINE_HPP

#include "snake.hpp"
#include "fruit.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>
#include <deque>

using namespace sf;
using namespace std;

constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;

class Engine {
    private:
        // Window
        RenderWindow window;
        const unsigned int FPS = 60;
        static const Time timePerFrame;

        // Snake
        vector<Snake> snake;
        int snakeDirection;
        deque<int> direction;
        int speed;
        int sectionToAdd; 

        // Fruit
        Fruit fruit;

        Time timeSinceLastMove;

        int currentGameState; 
        int lastGameState;

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