#include "MenuManager.h"
#include <sstream>
#include <memory>

namespace SnakeGame
{
    MenuManager::MenuManager(sf::Font& font, Leaderboard& leaderboard)
        : m_font(font)
        , m_leaderboard(leaderboard)
        , m_difficultyMenu(std::make_unique<DifficultyMenu>(font))  
    {
        m_overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        m_overlay.setFillColor(sf::Color(0, 0, 0, 200));

        m_backText.setFont(m_font);
        m_backText.setString("Press ESC to return");
        m_backText.setCharacterSize(20);
        m_backText.setFillColor(sf::Color::White);

        SetupMainMenu();
        SetupGameMenu();
    }

    void MenuManager::CenterText(sf::Text& text, float y)
    {
        if (text.getFont() == nullptr) return;
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(SCREEN_WIDTH / 2.0f, y);
    }

    void MenuManager::SetupMainMenu()
    {
        m_mainMenuTitle.setFont(m_font);
        m_mainMenuTitle.setString("SNAKE GAME");
        m_mainMenuTitle.setCharacterSize(72);
        m_mainMenuTitle.setFillColor(sf::Color::Yellow);
        CenterText(m_mainMenuTitle, 120.0f);

        std::vector<std::string> items = {
            "START GAME",
            "DIFFICULTY",
            "LEADERBOARD",
            "EXIT"
        };

        m_mainMenuItems.clear();
        float startY = 250.0f;
        float spacing = 70.0f;

        for (size_t i = 0; i < items.size(); ++i)
        {
            sf::Text text;
            text.setFont(m_font);
            text.setString(items[i]);
            text.setCharacterSize(36);
            text.setFillColor(i == static_cast<size_t>(m_selectedMainItem) ? sf::Color::Yellow : sf::Color::White);
            CenterText(text, startY + i * spacing);
            m_mainMenuItems.push_back(text);
        }
    }

    void MenuManager::SetupGameMenu()
    {
        m_gameMenuTitle.setFont(m_font);
        m_gameMenuTitle.setString("GAME MENU");
        m_gameMenuTitle.setCharacterSize(36);
        m_gameMenuTitle.setFillColor(sf::Color::Yellow);
        CenterText(m_gameMenuTitle, 150.0f);

        std::vector<std::string> items = {
            "CONTINUE",
            "RESTART",
            "LEADERBOARD",
            "EXIT TO MENU"
        };

        m_gameMenuItems.clear();
        float startY = 250.0f;
        float spacing = 60.0f;

        for (size_t i = 0; i < items.size(); ++i)
        {
            sf::Text text;
            text.setFont(m_font);
            text.setString(items[i]);
            text.setCharacterSize(36);
            text.setFillColor(i == static_cast<size_t>(m_selectedGameItem) ? sf::Color::Yellow : sf::Color::White);
            CenterText(text, startY + i * spacing);
            m_gameMenuItems.push_back(text);
        }
    }

    void MenuManager::UpdateSoundsState(bool soundsEnabled, bool musicEnabled)
    {
        m_soundsEnabled = soundsEnabled;
        m_musicEnabled = musicEnabled;
    }

    void MenuManager::DrawMainMenu(sf::RenderWindow& window)
    {
        window.draw(m_mainMenuTitle);
        for (const auto& item : m_mainMenuItems)
            window.draw(item);
    }

    void MenuManager::DrawDifficultyMenu(sf::RenderWindow& window, Difficulty currentDifficulty)
    {
        if (m_difficultyMenu)
            m_difficultyMenu->Draw(window, currentDifficulty);
    }

    void MenuManager::DrawGameMenu(sf::RenderWindow& window)
    {
        window.draw(m_overlay);
        window.draw(m_gameMenuTitle);
        for (const auto& item : m_gameMenuItems)
            window.draw(item);
    }

    void MenuManager::DrawLeaderboard(sf::RenderWindow& window)
    {
        window.draw(m_overlay);

        sf::Text leaderText;
        leaderText.setFont(m_font);
        leaderText.setString(m_leaderboard.Format());
        leaderText.setCharacterSize(24);
        leaderText.setFillColor(sf::Color::Cyan);
        CenterText(leaderText, 150.0f);
        window.draw(leaderText);

        sf::FloatRect backRect = m_backText.getLocalBounds();
        m_backText.setOrigin(backRect.left + backRect.width / 2.0f, backRect.top + backRect.height / 2.0f);
        m_backText.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 50.0f);
        window.draw(m_backText);
    }

    void MenuManager::MoveMainMenuUp()
    {
        m_selectedMainItem = (m_selectedMainItem - 1 + static_cast<int>(MainMenuItem::COUNT)) % static_cast<int>(MainMenuItem::COUNT);
        SetupMainMenu();
    }

    void MenuManager::MoveMainMenuDown()
    {
        m_selectedMainItem = (m_selectedMainItem + 1) % static_cast<int>(MainMenuItem::COUNT);
        SetupMainMenu();
    }

    void MenuManager::MoveDifficultyUp()
    {
        if (m_difficultyMenu)
            m_difficultyMenu->MoveUp();
    }

    void MenuManager::MoveDifficultyDown()
    {
        if (m_difficultyMenu)
            m_difficultyMenu->MoveDown();
    }

    void MenuManager::MoveGameMenuUp()
    {
        m_selectedGameItem = (m_selectedGameItem - 1 + static_cast<int>(GameMenuItem::COUNT)) % static_cast<int>(GameMenuItem::COUNT);
        SetupGameMenu();
    }

    void MenuManager::MoveGameMenuDown()
    {
        m_selectedGameItem = (m_selectedGameItem + 1) % static_cast<int>(GameMenuItem::COUNT);
        SetupGameMenu();
    }

    void MenuManager::SetDifficulty(Difficulty diff)
    {
        // Сохраняем выбранную сложность
    }

    Difficulty MenuManager::GetCurrentDifficulty() const
    {
        return Difficulty::MEDIUM;
    }
}