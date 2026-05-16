#include "DifficultyMenu.h"

namespace SnakeGame
{
    DifficultyMenu::DifficultyMenu(sf::Font& font)
        : m_font(font)
    {
        m_overlay.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        m_overlay.setFillColor(sf::Color(0, 0, 0, 220));

        Setup();
    }

    void DifficultyMenu::CenterText(sf::Text& text, float y)
    {
        if (text.getFont() == nullptr)
            return;

        text.setOrigin(0.0f, 0.0f);
        sf::FloatRect bounds = text.getLocalBounds();

        float centerX = SCREEN_WIDTH / 2.0f - (bounds.left + bounds.width / 2.0f);
        text.setPosition(centerX, y);
    }

    void DifficultyMenu::CenterText1(sf::Text& text, float y)
    {
        if (text.getFont() == nullptr) return;
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        text.setPosition(SCREEN_WIDTH / 2.0f, y);
    }

    void DifficultyMenu::Setup()
    {
        // Заголовок
        m_title.setFont(m_font);
        m_title.setString("SELECT DIFFICULTY");
        m_title.setCharacterSize(30);          
        m_title.setFillColor(sf::Color::Yellow);
        m_title.setStyle(sf::Text::Bold);
        CenterText(m_title, 85.0f);

        // Пункты меню
        m_items.clear();
        float startY = 160.0f;
        float spacing = 60.0f;

        for (int i = 0; i < 5; ++i)
        {
            sf::Text text;
            text.setFont(m_font);
            text.setString(m_difficulties[i].name);
            text.setCharacterSize(24);
            text.setFillColor(i == m_selectedItem ?
                m_difficulties[i].color : sf::Color(175, 175, 175));

            CenterText(text, startY + i * spacing);
            m_items.push_back(text);
        }
    }

    void DifficultyMenu::Draw(sf::RenderWindow& window, Difficulty currentDifficulty)
    {
        window.draw(m_overlay);
        window.draw(m_title);

        for (const auto& item : m_items)
            window.draw(item);

        // Маркер выбора
        if (m_selectedItem >= 0 && m_selectedItem < static_cast<int>(m_items.size()))
        {
            sf::Text marker;
            marker.setFont(m_font);
            marker.setString(">");
            marker.setCharacterSize(24);
            marker.setFillColor(sf::Color::Green);
            marker.setStyle(sf::Text::Bold);

            sf::FloatRect bounds = marker.getLocalBounds();
            marker.setOrigin(0.0f, bounds.top + bounds.height / 2.0f);

            float markerX = m_items[m_selectedItem].getPosition().x - 105.0f;
            float markerY = m_items[m_selectedItem].getPosition().y + 10.0f;

            marker.setPosition(markerX, markerY);
            window.draw(marker);
        }

        // Описание сложности
        if (m_selectedItem >= 0 && m_selectedItem < 5)
        {
            sf::Text desc;
            desc.setFont(m_font);
            desc.setString(m_difficulties[m_selectedItem].description);
            desc.setCharacterSize(20);
            desc.setFillColor(sf::Color::White);
            CenterText(desc, 480.0f);
            window.draw(desc);
        }

        // Подсказка внизу
        sf::Text hint;
        hint.setFont(m_font);
        hint.setString("^ V - Navigate | Enter - Start Game | ESC - Back");
        hint.setCharacterSize(18);
        hint.setFillColor(sf::Color(200, 200, 200));
        CenterText(hint, 570.0f);
        window.draw(hint);
    }

    void DifficultyMenu::MoveUp()
    {
        m_selectedItem = (m_selectedItem - 1 + 5) % 5;
        UpdateColors();
    }

    void DifficultyMenu::MoveDown()
    {
        m_selectedItem = (m_selectedItem + 1) % 5;
        UpdateColors();
    }

    void DifficultyMenu::UpdateColors()
    {
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            m_items[i].setFillColor(i == static_cast<size_t>(m_selectedItem) ?
                m_difficulties[i].color : sf::Color(175, 175, 175));
        }
    }

    Difficulty DifficultyMenu::GetSelectedDifficulty() const
    {
        return static_cast<Difficulty>(m_selectedItem);
    }

    void DifficultyMenu::ResetSelection()
    {
        m_selectedItem = 0;
        UpdateColors();
    }
}