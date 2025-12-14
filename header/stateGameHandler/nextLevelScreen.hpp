#pragma once 

#include <SFML/Graphics.hpp>
#include "iscreen.hpp"
#include <optional>
#include "resource_path.h"

using namespace sf;
using namespace std;

class NextLevelScreen : public IScreen {
    private:
        Font font;
        optional<Text> nextLevelText;
        RectangleShape overlay;  
        bool fontLoaded;

        optional<Text> restartText;
        optional<Text> continueText;
        optional<Text> quitText;
        int selectedIndex = 0;

    public:
        NextLevelScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;        
};