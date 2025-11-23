#pragma once

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>

using namespace sf;

class MainMenuScreen : public IScreen {
    private:
        Font font;
        std::optional<Text> titleText;
        std::optional<Text> instructionText;
        bool fontLoaded;

    public:
        MainMenuScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
};