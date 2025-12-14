#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "iscreen.hpp"
#include "levelMap.hpp"
#include "resource_path.h"

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

        // Background 
        VertexArray grassBackgroundVA;
        VertexArray wallBackgroundVA;
        VertexArray enemiesBackgroundVA;
        Texture backgroundTileset;
        bool backgroundLoaded;

    public:
        MainMenuScreen();
        
        void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) override;
        void update(Engine& engine, Time dt) override;
        void draw(Engine& engine) override;
};