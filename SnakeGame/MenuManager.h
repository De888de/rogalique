#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <memory>
#include "Leaderboard.h"
#include "Constants.h"  
#include "DifficultyMenu.h"

namespace SnakeGame
{
    enum class MainMenuItem
    {
        START_GAME,
        DIFFICULTY,
        LEADERBOARD,
        EXIT,
        COUNT
    };

    enum class GameMenuItem
    {
        CONTINUE,
        RESTART,
        LEADERBOARD,
        EXIT_TO_MENU,
        COUNT
    };

    class MenuManager
    {
    private:
        sf::Font& m_font;
        Leaderboard& m_leaderboard;

        // Главное меню
        sf::Text m_mainMenuTitle;
        std::vector<sf::Text> m_mainMenuItems;
        int m_selectedMainItem = 0;

        // Игровое меню
        sf::Text m_gameMenuTitle;
        std::vector<sf::Text> m_gameMenuItems;
        int m_selectedGameItem = 0;

        // Общие элементы
        sf::Text m_backText;
        sf::RectangleShape m_overlay;

        // Состояния звука
        bool m_soundsEnabled = true;
        bool m_musicEnabled = true;

        // Указатель на меню сложности 
        std::unique_ptr<DifficultyMenu> m_difficultyMenu;

    public:
        MenuManager(sf::Font& font, Leaderboard& leaderboard);

        void UpdateSoundsState(bool soundsEnabled, bool musicEnabled);
        void SetDifficulty(Difficulty diff);
        Difficulty GetCurrentDifficulty() const;

        void DrawMainMenu(sf::RenderWindow& window);
        void DrawDifficultyMenu(sf::RenderWindow& window, Difficulty currentDifficulty);
        void DrawGameMenu(sf::RenderWindow& window);
        void DrawLeaderboard(sf::RenderWindow& window);

        void MoveMainMenuUp();
        void MoveMainMenuDown();
        void MoveDifficultyUp();
        void MoveDifficultyDown();
        void MoveGameMenuUp();
        void MoveGameMenuDown();

        MainMenuItem GetSelectedMainItem() const { return static_cast<MainMenuItem>(m_selectedMainItem); }
        GameMenuItem GetSelectedGameItem() const { return static_cast<GameMenuItem>(m_selectedGameItem); }

        void ResetDifficultySelection() { if (m_difficultyMenu) m_difficultyMenu->ResetSelection(); }
        Difficulty GetSelectedDifficulty() const { return m_difficultyMenu ? m_difficultyMenu->GetSelectedDifficulty() : Difficulty::MEDIUM; }

    private:
        void SetupMainMenu();
        void SetupGameMenu();
        void CenterText(sf::Text& text, float y);
    };
}