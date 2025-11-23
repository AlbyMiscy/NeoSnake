#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Engine; 

class IScreen {
    public:
        virtual ~IScreen() = default;

        virtual void handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent) = 0;
        virtual void update(Engine& engine, Time dt) = 0;
        virtual void draw(Engine& engine) = 0;
};