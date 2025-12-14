#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "iscreen.hpp"
#include "resource_path.h"

using namespace sf;
using namespace std;

class PauseScreen : public IScreen {
    private:
        Font font;
        optional<Text> pauseText;
        RectangleShape overlay;  
        bool fontLoaded;

        optional<Text> resumeText;
        optional<Text> escText;
        int selectedIndex = 0;

    public:
        PauseScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
};
