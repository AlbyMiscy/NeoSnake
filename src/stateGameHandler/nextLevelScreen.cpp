#include "nextLevelScreen.hpp"
#include "engine.hpp"

NextLevelScreen::NextLevelScreen()
: fontLoaded(false),
selectedIndex(0)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) {
        fontLoaded = true;

        nextLevelText.emplace(font);
        nextLevelText->setString("CONGRATULATIONS!");
        nextLevelText->setCharacterSize(50);
        nextLevelText->setFillColor(Color::Green);

        continueText.emplace(font);
        continueText->setString("Continue");
        continueText->setCharacterSize(35);
        continueText->setFillColor(Color::Yellow); // default

        restartText.emplace(font);
        restartText->setString("Restart");
        restartText->setCharacterSize(35);
        restartText->setFillColor(Color::White);
        
        quitText.emplace(font);
        quitText->setString("Quit");
        quitText->setCharacterSize(35);
        quitText->setFillColor(Color::White);
    }

    overlay.setFillColor(Color(0, 0, 0, 180));
}

void NextLevelScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    const int menuItemCount = 3; // CONTINUE, RESTART, QUIT

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
                // CONTINUE - passa al livello successivo
                engine.continueToNextLevel();
            } else if (selectedIndex == 1) {
                // RESTART - riavvia il livello corrente
                engine.restartCurrentLevel();
            } else {
                // QUIT - esci dal gioco
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

void NextLevelScreen::update(Engine&, Time)
{
}

void NextLevelScreen::draw(Engine& engine)
{
    if (!fontLoaded) return;

    auto& window = engine.getWindow();
    
    View oldView = window.getView();
    window.setView(engine.getUIView());

    Vector2u size = window.getSize();

    // Disegna l'overlay semi-trasparente
    overlay.setSize(Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
    window.draw(overlay);

    // Aggiorna i colori in base alla selezione
    if (continueText && restartText && quitText) {
        continueText->setFillColor(selectedIndex == 0 ? Color::Yellow : Color::White);
        restartText->setFillColor(selectedIndex == 1 ? Color::Yellow : Color::White);
        quitText->setFillColor(selectedIndex == 2 ? Color::Yellow : Color::White);
    }

    // Disegna il testo "CONGRATULATIONS!"
    if (nextLevelText) {
        FloatRect bounds = nextLevelText->getLocalBounds();
        nextLevelText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        nextLevelText->setPosition(
            { static_cast<float>(size.x) / 2.f,
              static_cast<float>(size.y) / 4.f }
        );
        window.draw(*nextLevelText);
    }

    // Disegna le opzioni del menu
    const float centerX = static_cast<float>(size.x) / 2.f;
    const float startY = static_cast<float>(size.y) / 2.f;

    if (continueText) {
        FloatRect b = continueText->getLocalBounds();
        continueText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        continueText->setPosition({ centerX, startY });
        window.draw(*continueText);
    }

    if (restartText) {
        FloatRect b = restartText->getLocalBounds();
        restartText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        restartText->setPosition({ centerX, startY + 60.f });
        window.draw(*restartText);
    }

    if (quitText) {
        FloatRect b = quitText->getLocalBounds();
        quitText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        quitText->setPosition({ centerX, startY + 120.f });
        window.draw(*quitText);
    }

    window.setView(oldView);
}