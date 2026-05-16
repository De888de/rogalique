#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.h"  

namespace SnakeGame
{
    class DifficultyMenu
    {
    private:
        sf::Font& m_font;
        sf::Text m_title;
        std::vector<sf::Text> m_items;
        int m_selectedItem = 0;
        sf::RectangleShape m_overlay;

        struct DifficultyInfo
        {
            const char* name;
            const char* description;
            sf::Color color;
        };

        const DifficultyInfo m_difficulties[5] = {
            {"EASY", " Slow speed\n No rocks\n Perfect for beginners", sf::Color::Green},
            {"MEDIUM", " Normal speed\n 10 rocks\n Classic challenge", sf::Color::Yellow},
            {"HARD", " Fast speed\n 30 rocks\n For experienced players + Magic apples", sf::Color(255, 140, 0)},
            {"INSANE", " Very fast\n 50 rocks\n Portals appear + Magic apples", sf::Color::Red},
            {"MAGICAL", " Maximum speed\n 70 rocks\n Portals + Magic apples", sf::Color::Magenta}
        };

        // Центрирование текста по ширине окна
        void CenterText(sf::Text& text, float y);
        void CenterText1(sf::Text& text, float y);

    public:
        DifficultyMenu(sf::Font& font);

        void Draw(sf::RenderWindow& window, Difficulty currentDifficulty);
        void MoveUp();
        void MoveDown();
        Difficulty GetSelectedDifficulty() const;
        void ResetSelection();

    private:
        void Setup();
        void UpdateColors();
    };
}