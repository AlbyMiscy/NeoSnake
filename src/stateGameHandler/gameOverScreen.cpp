#include "gameOverScreen.hpp"
#include "engine.hpp"
#include "resource_path.h"
#include <sstream>

GameOverScreen::GameOverScreen()
: fontLoaded(false)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) {
        fontLoaded = true;

        gameOverText.emplace(font);
        gameOverText->setString("GAME OVER");
        gameOverText->setCharacterSize(40);
        gameOverText->setFillColor(sf::Color::Red);

        scoreText.emplace(font);
        scoreText->setCharacterSize(24);
        scoreText->setFillColor(sf::Color::White);

        instructionText.emplace(font);
        instructionText->setString("Invio: ricomincia\nESC: esci");
        instructionText->setCharacterSize(20);
        instructionText->setFillColor(sf::Color::White);
    }

    overlay.setFillColor(sf::Color(0, 0, 0, 180));
}

void GameOverScreen::setScore(int score)
{
    if (!scoreText) return;
    std::ostringstream oss;
    oss << "Punteggio: " << score;
    scoreText->setString(oss.str());
}

void GameOverScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    if (keyEvent.scancode == Keyboard::Scancode::Enter) {
        engine.startGame();
    }
    else if (keyEvent.scancode == Keyboard::Scancode::Escape) {
        engine.getWindow().close();
    }
}

void GameOverScreen::update(Engine&, Time)
{
    // eventuali animazioni (lampeggio, ecc.)
}

void GameOverScreen::draw(Engine& engine)
{
    if (!fontLoaded) return;
    auto& window = engine.getWindow();

    sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    sf::Vector2u size = window.getSize();
    overlay.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));

    window.draw(overlay);

    if (gameOverText) {
        sf::FloatRect b = gameOverText->getLocalBounds();
        gameOverText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        gameOverText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 3.f});
        window.draw(*gameOverText);
    }

    if (scoreText) {
        sf::FloatRect b = scoreText->getLocalBounds();
        scoreText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        scoreText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
        window.draw(*scoreText);
    }

    if (instructionText) {
        sf::FloatRect b = instructionText->getLocalBounds();
        instructionText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        instructionText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) * 2.f / 3.f});
        window.draw(*instructionText);
    }

    window.setView(oldView);
}
