#include "gameOverScreen.hpp"
#include "engine.hpp"

GameOverScreen::GameOverScreen()
: fontLoaded(false),
selectedIndex(0)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) {
        fontLoaded = true;

        gameOverText.emplace(font);
        gameOverText->setString("GAME OVER");
        gameOverText->setCharacterSize(50);
        gameOverText->setFillColor(Color::Red);

        scoreText.emplace(font);
        scoreText->setCharacterSize(24);
        scoreText->setFillColor(Color::White);

        restartText.emplace(font);
        restartText->setString("Restart");
        restartText->setCharacterSize(35);
        restartText->setFillColor(Color::Yellow); // default

        quitText.emplace(font);
        quitText->setString("Quit");
        quitText->setCharacterSize(35);
        quitText->setFillColor(Color::White);
    }

    overlay.setFillColor(Color(0, 0, 0, 180));
}

void GameOverScreen::setScore(int score)
{
    if (!scoreText) return;
    ostringstream oss;
    oss << "Score: " << score;
    scoreText->setString(oss.str());
}

void GameOverScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    const int menuItemCount = 2; // RESTART, QUIT

    switch (keyEvent.scancode)
    {
        case Keyboard::Scancode::Up:
            selectedIndex = (selectedIndex - 1 + menuItemCount) % menuItemCount;
            break;

        case Keyboard::Scancode::Down:
            selectedIndex = (selectedIndex + 1) % menuItemCount;
            break;

        case Keyboard::Scancode::Enter:
            if (selectedIndex == 0) {
                // RESTART - riavvia il livello corrente con score a 0
                engine.restartCurrentLevel();
            } else {
                // QUIT
                engine.getWindow().close();
            }
            break;
        
        case Keyboard::Scancode::Escape:
            engine.getWindow().close();
            break;


        default:
            break;
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

    View oldView = window.getView();
    window.setView(engine.getUIView());

    Vector2u size = window.getSize();
    overlay.setSize(Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    window.draw(overlay);

    const float centerX = static_cast<float>(size.x) / 2.f;

    if (restartText && quitText) {
        if (selectedIndex == 0) {
            restartText->setFillColor(Color::Yellow);
            quitText->setFillColor(Color::White);
        } else {
            restartText->setFillColor(Color::White);
            quitText->setFillColor(Color::Yellow);
        }
    }

    // title
    if (gameOverText) {
        FloatRect b = gameOverText->getLocalBounds();
        gameOverText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        gameOverText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 3.f }
        );
        window.draw(*gameOverText);
    }

    // Score
    if (scoreText) {
        FloatRect b = scoreText->getLocalBounds();
        scoreText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        scoreText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f - 40.f }
        );
        window.draw(*scoreText);
    }

    // Restart option
    if (restartText) {
        FloatRect b = restartText->getLocalBounds();
        restartText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        restartText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f + 20.f }
        );
        window.draw(*restartText);
    }

    // Quit option 
    if (quitText) {
        FloatRect b = quitText->getLocalBounds();
        quitText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        quitText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f + 80.f }
        );
        window.draw(*quitText);
    }

    window.setView(oldView);
}
