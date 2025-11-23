#pragma once

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>

using namespace sf;
using namespace std;

class GameOverScreen : public IScreen {
    private:
        Font font;
        optional<Text> gameOverText;
        optional<Text> scoreText;
        RectangleShape overlay;  
        bool fontLoaded;

        optional<Text> restartText;
        optional<Text> quitText;
        int selectedIndex = 0;

    public:
        GameOverScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
        
        // TODO
        void setScore(int score); 
};
