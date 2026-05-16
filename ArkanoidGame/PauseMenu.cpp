#include "PauseMenu.h"
#include "GameSettings.h"

namespace ArkanoidGame
{
    PauseMenu::PauseMenu()
    {
        InitText();
    }

    void PauseMenu::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        titleText.setFont(font);
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setString("PAUSED");
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - titleBounds.width / 2.f, 100.f);

        resumeText.setFont(font);
        resumeText.setCharacterSize(36);
        resumeText.setFillColor(sf::Color::White);
        resumeText.setString("Resume");
        sf::FloatRect resumeBounds = resumeText.getLocalBounds();
        resumeText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - resumeBounds.width / 2.f, 200.f);

        saveExitText.setFont(font);
        saveExitText.setCharacterSize(36);
        saveExitText.setFillColor(sf::Color::White);
        saveExitText.setString("Save & Exit");
        sf::FloatRect saveExitBounds = saveExitText.getLocalBounds();
        saveExitText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - saveExitBounds.width / 2.f, 280.f);

        restartText.setFont(font);
        restartText.setCharacterSize(36);
        restartText.setFillColor(sf::Color::White);
        restartText.setString("Restart Level");
        sf::FloatRect restartBounds = restartText.getLocalBounds();
        restartText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - restartBounds.width / 2.f, 360.f);
    }

    void PauseMenu::Open()
    {
        isOpen = true;
        selectedIndex = 0;
        UpdateSelection();
    }

    void PauseMenu::Close()
    {
        isOpen = false;
    }

    void PauseMenu::MoveUp()
    {
        if (selectedIndex > 0)
            selectedIndex--;
        else
            selectedIndex = totalOptions - 1;
        UpdateSelection();
    }

    void PauseMenu::MoveDown()
    {
        if (selectedIndex < totalOptions - 1)
            selectedIndex++;
        else
            selectedIndex = 0;
        UpdateSelection();
    }

    void PauseMenu::UpdateSelection()
    {
        resumeText.setFillColor(sf::Color::White);
        saveExitText.setFillColor(sf::Color::White);
        restartText.setFillColor(sf::Color::White);

        switch (selectedIndex)
        {
        case 0: resumeText.setFillColor(sf::Color::Yellow); break;
        case 1: saveExitText.setFillColor(sf::Color::Yellow); break;
        case 2: restartText.setFillColor(sf::Color::Yellow); break;
        }
    }

    void PauseMenu::ExecuteCurrent()
    {
        switch (selectedIndex)
        {
        case 0: if (onResume) onResume(); break;
        case 1: if (onSaveAndExit) onSaveAndExit(); break;
        case 2: if (onRestartLevel) onRestartLevel(); break;
        }
    }

    void PauseMenu::Draw(sf::RenderWindow& window)
    {
        if (!isOpen) return;

        sf::RectangleShape overlay(sf::Vector2f(SETTINGS.SCREEN_WIDTH, SETTINGS.SCREEN_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        window.draw(overlay);

        window.draw(titleText);
        window.draw(resumeText);
        window.draw(saveExitText);
        window.draw(restartText);
    }

    void PauseMenu::HandleInput(const sf::Event& event)
    {
        if (!isOpen) return;

        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::Up:
                MoveUp();
                break;
            case sf::Keyboard::Down:
                MoveDown();
                break;
            case sf::Keyboard::Enter:
            case sf::Keyboard::Space:
                ExecuteCurrent();
                break;
            case sf::Keyboard::Escape:
                if (onResume) onResume();
                break;
            default:
                break;
            }
        }
    }
}
