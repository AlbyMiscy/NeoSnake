#include "mainMenuScreen.hpp"
#include "engine.hpp"
#include "resource_path.h"

MainMenuScreen::MainMenuScreen()
: fontLoaded(false)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) {
        fontLoaded = true;

        titleText.emplace(font);
        titleText->setString("NeoSnake");
        titleText->setCharacterSize(48);
        titleText->setFillColor(sf::Color::Green);

        instructionText.emplace(font);
        instructionText->setString("Premi INVIO per iniziare\nESC per uscire");
        instructionText->setCharacterSize(24);
        instructionText->setFillColor(sf::Color::White);
    }
}

void MainMenuScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    if (keyEvent.scancode == Keyboard::Scancode::Enter) {
        engine.startGame();
    }
    else if (keyEvent.scancode == Keyboard::Scancode::Escape) {
        engine.getWindow().close();
    }
}

void MainMenuScreen::update(Engine&, Time)
{
    // eventuali animazioni del menu
}

void MainMenuScreen::draw(Engine& engine)
{
    if (!fontLoaded) return;

    auto& window = engine.getWindow();

    // Usa la view di default per centrare bene il testo
    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    sf::Vector2u size = window.getSize();

    if (titleText) {
        sf::FloatRect bounds = titleText->getLocalBounds();
        titleText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        titleText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 3.f});
        window.draw(*titleText);
    }

    if (instructionText) {
        sf::FloatRect bounds = instructionText->getLocalBounds();
        instructionText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        instructionText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) * 2.f / 3.f});
        window.draw(*instructionText);
    }

    window.setView(oldView);
}
