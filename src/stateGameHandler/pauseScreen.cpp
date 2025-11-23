#include "pauseScreen.hpp"
#include "engine.hpp"
#include "resource_path.h"

PauseScreen::PauseScreen()
: fontLoaded(false)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) { 
        fontLoaded = true;

        pauseText.emplace(font);
        pauseText->setString("PAUSA");
        pauseText->setCharacterSize(40);
        pauseText->setFillColor(sf::Color::Yellow);

        instructionText.emplace(font);
        instructionText->setString("Premi P per riprendere\nESC per uscire");
        instructionText->setCharacterSize(20);
        instructionText->setFillColor(sf::Color::White);
    }

    overlay.setFillColor(sf::Color(0, 0, 0, 150));
}

void PauseScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    if (keyEvent.scancode == Keyboard::Scancode::P) {
        // torna allo stato precedente (RUNNING)
        engine.togglePause();
    }
    else if (keyEvent.scancode == Keyboard::Scancode::Escape) {
        engine.getWindow().close();
    }
}

void PauseScreen::update(Engine&, Time)
{
    // niente per ora
}

void PauseScreen::draw(Engine& engine)
{
    if (!fontLoaded) return;
    auto& window = engine.getWindow();

    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    sf::Vector2u size = window.getSize();
    overlay.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));

    window.draw(overlay);

    if (pauseText) {
        sf::FloatRect b = pauseText->getLocalBounds();
        pauseText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        pauseText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 3.f});
        window.draw(*pauseText);
    }

    if (instructionText) {
        sf::FloatRect b = instructionText->getLocalBounds();
        instructionText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        instructionText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) * 2.f / 3.f});
        window.draw(*instructionText);
    }

    window.setView(oldView);
}
