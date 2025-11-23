#pragma once

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>

using namespace sf;

class GameOverScreen : public IScreen {
    private:
        Font font;
        std::optional<Text> gameOverText;
        std::optional<Text> scoreText;
        std::optional<Text> instructionText;
        RectangleShape overlay;  // Overlay semi-trasparente
        bool fontLoaded;

    public:
        GameOverScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
        
        void setScore(int score);  // Per aggiornare il punteggio visualizzato
};
