#pragma once

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>

using namespace sf;
using namespace std;

class MainMenuScreen : public IScreen {
    private:
        Font font;
        optional<Text> titleText;
        bool fontLoaded;

        optional<Text> startText;
        optional<Text> quitText;
        int selectedIndex = 0;

    public:
        MainMenuScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
};