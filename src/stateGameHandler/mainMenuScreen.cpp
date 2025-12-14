#include "mainMenuScreen.hpp"
#include "engine.hpp"

MainMenuScreen::MainMenuScreen()
: fontLoaded(false),
selectedIndex(0),
backgroundLoaded(false)
{
    if (backgroundTileset.loadFromFile(RESOURCE_DIR "/texture/menu_tileset.png")) {
        backgroundTileset.setSmooth(false); // Disabilita smoothing per evitare texture bleeding
        string grassPath = string(RESOURCE_DIR) + "/backgroundMenu/menuBackground_grass_background.csv";
        string wallPath = string(RESOURCE_DIR) + "/backgroundMenu/menuBackground_wall_background.csv";
        string enemiesPath = string(RESOURCE_DIR) + "/backgroundMenu/menuBackground_enemies_background.csv";
        
        bool grassLoaded = LevelMap::buildBackgroundFromCsv(grassBackgroundVA, grassPath, backgroundTileset, 20.f);
        bool wallLoaded = LevelMap::buildBackgroundFromCsv(wallBackgroundVA, wallPath, backgroundTileset, 20.f);
        bool enemiesLoaded = LevelMap::buildBackgroundFromCsv(enemiesBackgroundVA, enemiesPath, backgroundTileset, 20.f);
        
        backgroundLoaded = grassLoaded && enemiesLoaded && wallLoaded;
    }
    
    if (font.openFromFile(RESOURCE_DIR "/fonts/PressStart2P-Regular.ttf")) {
        fontLoaded = true;

        // Title
        titleText.emplace(font);
        titleText->setString("NeoSnake");
        titleText->setCharacterSize(55);
        titleText->setFillColor(Color(0, 180, 0)); // Verde scuro

        // start option
        startText.emplace(font);
        startText->setString("Start");
        startText->setCharacterSize(35);
        startText->setFillColor(Color::Yellow); // default
        
        // quit option
        quitText.emplace(font);
        quitText->setString("Quit");
        quitText->setCharacterSize(35);
        quitText->setFillColor(Color::White);
    }
}

void MainMenuScreen::handleKeyPressed(Engine& engine, const Event::KeyPressed& keyEvent)
{
    const int menuItemCount = 2; // START, QUIT

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
                // START
                engine.startGame();
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

void MainMenuScreen::update(Engine&, Time)
{
}

void MainMenuScreen::draw(Engine& engine)
{
    if (!fontLoaded) return;

    auto& window = engine.getWindow();

    View oldView = window.getView();
    window.setView(engine.getUIView());

    // Draw background layers (grass first, enemies, then wall on top)
    if (backgroundLoaded) {
        // Calculate scale to fit background to current view size
        Vector2u windowSize = window.getSize();
        float scaleX = static_cast<float>(windowSize.x) / 1280.f; // Original design width
        float scaleY = static_cast<float>(windowSize.y) / 720.f;  // Original design height
        
        RenderStates states;
        states.texture = &backgroundTileset;
        states.transform.scale({scaleX, scaleY});
        
        window.draw(grassBackgroundVA, states);
        window.draw(enemiesBackgroundVA, states);
        window.draw(wallBackgroundVA, states);
    }

    Vector2u size = window.getSize();

    // update color
    if (startText && quitText) {
        if (selectedIndex == 0) {
            startText->setFillColor(Color::Yellow);
            quitText->setFillColor(Color::White);
        } else {
            startText->setFillColor(Color::White);
            quitText->setFillColor(Color::Yellow);
        }
    }

    // Title
    if (titleText) {
        FloatRect bounds = titleText->getLocalBounds();
        titleText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        titleText->setPosition(
            { static_cast<float>(size.x) / 2.f,
              static_cast<float>(size.y) / 4.f }
        );
        window.draw(*titleText);
    }

    // Start and Quit options
    const float centerX = static_cast<float>(size.x) / 2.f;

    if (startText) {
        FloatRect b = startText->getLocalBounds();
        startText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        startText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f }
        );
        window.draw(*startText);
    }

    if (quitText) {
        FloatRect b = quitText->getLocalBounds();
        quitText->setOrigin({b.size.x / 2.f, b.size.y / 2.f});
        quitText->setPosition(
            { centerX,
              static_cast<float>(size.y) / 2.f + 60.f }
        );
        window.draw(*quitText);
    }

    window.setView(oldView);
}

