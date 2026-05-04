#include "Menu.h"
#include "Application.h"
#include "Game.h"
#include "GameSettings.h"
#include <iostream>

namespace ArkanoidGame {

    extern Application* g_Application;
    extern Game* g_Game;

    Menu::Menu()
    {
        InitText();
        if (g_Game)
        {
            hasSave = g_Game->HasSavedProgress();
            std::cout << "[Menu] Created, hasSave=" << hasSave << std::endl;
        }
        selectedIndex = 0;
        UpdateSelection();
    }

    void Menu::InitText()
    {
        if (!font.loadFromFile("Resources/Fonts/Roboto-Black.ttf"))
        {
            font.loadFromFile("D:/xyz/HW_03/xyz-cpp-course/ArkanoidGame/Resources/Fonts/Roboto-Black.ttf");
        }

        // Заголовок
        titleText.setFont(font);
        titleText.setCharacterSize(80);
        titleText.setFillColor(sf::Color::Yellow);
        titleText.setStyle(sf::Text::Bold);
        titleText.setString("ARKANOID");
        sf::FloatRect titleBounds = titleText.getLocalBounds();
        titleText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - titleBounds.width / 2.f, 100.f);

        // NEW GAME
        newGameText.setFont(font);
        newGameText.setCharacterSize(40);
        newGameText.setFillColor(sf::Color::White);
        newGameText.setString("NEW GAME");
        sf::FloatRect newGameBounds = newGameText.getLocalBounds();
        newGameText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - newGameBounds.width / 2.f, 250.f);

        // CONTINUE
        continueText.setFont(font);
        continueText.setCharacterSize(40);
        continueText.setFillColor(sf::Color::White);
        continueText.setString("CONTINUE");
        sf::FloatRect continueBounds = continueText.getLocalBounds();
        continueText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - continueBounds.width / 2.f, 330.f);

        // EXIT
        exitText.setFont(font);
        exitText.setCharacterSize(36);
        exitText.setFillColor(sf::Color::White);
        exitText.setString("EXIT");
        sf::FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setPosition(SETTINGS.SCREEN_WIDTH / 2.f - exitBounds.width / 2.f, 430.f);
    }

    void Menu::MoveUp()
    {
        if (selectedIndex > 0)
            selectedIndex--;
        else
            selectedIndex = totalOptions - 1;
        UpdateSelection();
    }

    void Menu::MoveDown()
    {
        if (selectedIndex < totalOptions - 1)
            selectedIndex++;
        else
            selectedIndex = 0;
        UpdateSelection();
    }

    void Menu::UpdateSelection()
    {
        // Сбрасываем цвета всех пунктов
        newGameText.setFillColor(sf::Color::White);
        continueText.setFillColor(sf::Color::White);
        exitText.setFillColor(sf::Color::White);

        // Подсвечиваем выбранный пункт
        switch (selectedIndex)
        {
        case 0: // NEW GAME
            newGameText.setFillColor(sf::Color::Yellow);
            break;
        case 1: // CONTINUE
            continueText.setFillColor(sf::Color::Yellow);
            break;
        case 2: // EXIT
            exitText.setFillColor(sf::Color::Yellow);
            break;
        }
    }

    void Menu::ExecuteCurrent()
    {
        if (!g_Game) return;

        switch (selectedIndex)
        {
        case 0: // NEW GAME
            std::cout << "[Menu] Starting NEW GAME..." << std::endl;
            g_Game->StartGame();
            if (g_Application)
                g_Application->StartGame();
            break;
        case 1: // CONTINUE
            if (hasSave)
            {
                std::cout << "[Menu] CONTINUE - loading saved game..." << std::endl;
                g_Game->ContinueGame();
                if (g_Application)
                    g_Application->StartGame();
            }
            else
            {
                // Если нет сохранения, начинаем новую игру
                std::cout << "[Menu] No save found, starting NEW GAME..." << std::endl;
                g_Game->StartGame();
                if (g_Application)
                    g_Application->StartGame();
            }
            break;
        case 2: // EXIT
            std::cout << "[Menu] EXIT - closing game..." << std::endl;
            if (g_Application)
                g_Application->window.close();
            break;
        }
    }

    void Menu::Update(float)
    {
        // Обновляем состояние сохранения каждый кадр
        if (g_Game)
        {
            bool currentSave = g_Game->HasSavedProgress();
            if (currentSave != hasSave)
            {
                hasSave = currentSave;
                std::cout << "[Menu] hasSave changed to: " << hasSave << std::endl;
            }
        }
    }

    void Menu::Draw(sf::RenderWindow& window)
    {
        window.draw(titleText);
        window.draw(newGameText);

        // Показываем CONTINUE только если есть сохранение
        if (hasSave)
            window.draw(continueText);

        window.draw(exitText);
    }

    void Menu::HandleWindowEvent(const sf::Event& event)
    {
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
                if (g_Application)
                    g_Application->window.close();
                break;
            default:
                break;
            }
        }
    }

    bool Menu::HasSave() const
    {
        return hasSave;
    }

    void Menu::UpdateHasSave(bool saveExists)
    {
        hasSave = saveExists;
        std::cout << "[Menu] UpdateHasSave: " << hasSave << std::endl;
    }

} // namespace ArkanoidGame
