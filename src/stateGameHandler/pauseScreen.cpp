#include "pauseScreen.hpp"
#include "engine.hpp"

PauseScreen::PauseScreen()
: fontLoaded(false),
selectedIndex(0)
{
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) { 
        fontLoaded = true;

        pauseText.emplace(font);
        pauseText->setString("PAUSE");
        pauseText->setCharacterSize(50);
        pauseText->setFillColor(Color::Yellow);

        // resume option
        resumeText.emplace(font);
        resumeText->setString("Resume");
        resumeText->setCharacterSize(35);
        resumeText->setFillColor(Color::Yellow); // default
        
        // quit option
        escText.emplace(font);
        escText->setString("Quit");
        escText->setCharacterSize(35);
        escText->setFillColor(Color::White); 
    }

    overlay.setFillColor(Color(0, 0, 0, 150));
}

void PauseScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    const int menuItemCount = 2; // RESUME, QUIT

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
                // RESUME
                engine.togglePause();
            } else {
                // QUIT
                engine.getWindow().close();
            }
            break;

        case Keyboard::Scancode::P:
            engine.togglePause();
            break;

        case Keyboard::Scancode::Escape:
            engine.getWindow().close();
            break;

        default:
            break;
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

    View oldView = window.getView();
    window.setView(engine.getUIView());

    Vector2u size = window.getSize();
    overlay.setSize(Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));

    // Resume and Esc options
    const float centerX = static_cast<float>(size.x) / 2.f;

    window.draw(overlay);

    if (resumeText && escText) {
        if (selectedIndex == 0) {
            resumeText->setFillColor(Color::Yellow);
            escText->setFillColor(Color::White);
        } else {
            resumeText->setFillColor(Color::White);
            escText->setFillColor(Color::Yellow);
        }
    }

    if (pauseText) {
        FloatRect b = pauseText->getLocalBounds();
        pauseText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        pauseText->setPosition({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 3.f});
        window.draw(*pauseText);
    }

    if (resumeText) {
        FloatRect b = resumeText->getLocalBounds();
        resumeText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        resumeText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f }
        );
        window.draw(*resumeText);
    }

    if (escText) {
        FloatRect b = escText->getLocalBounds();
        escText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        escText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f + 60.f }
        );
        window.draw(*escText);
    }

    window.setView(oldView);
}
