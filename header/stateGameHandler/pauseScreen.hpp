#pragma once

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>

using namespace sf;

class PauseScreen : public IScreen {
    private:
        Font font;
        std::optional<Text> pauseText;
        std::optional<Text> instructionText;
        RectangleShape overlay;  // Overlay semi-trasparente
        bool fontLoaded;

    public:
        PauseScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
};
