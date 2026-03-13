#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

namespace SnakeGame
{
    class SoundPopup
    {
    private:
        sf::Font& m_font;
        sf::RectangleShape m_background;
        sf::Text m_title;
        sf::Text m_musicText;
        sf::Text m_soundsText;
        sf::Text m_hint;

        int m_selectedItem = 0;  // 0 - музыка, 1 - звуки
        bool m_musicEnabled = true;
        bool m_soundsEnabled = true;

        // Для confirmation диалогов
        bool m_waitingForConfirm = false;
        int m_confirmItem = 0;
        sf::Text m_confirmText;

    public:
        SoundPopup(sf::Font& font)
            : m_font(font)
        {
            // Фон попапа
            m_background.setSize(sf::Vector2f(500, 300));
            m_background.setFillColor(sf::Color(30, 30, 30, 240));
            m_background.setOutlineColor(sf::Color::Yellow);
            m_background.setOutlineThickness(2.0f);
            m_background.setPosition(
                SCREEN_WIDTH / 2.0f - 250,
                SCREEN_HEIGHT / 2.0f - 150
            );

            Setup();
        }

        void Setup()
        {
            // Заголовок
            m_title.setFont(m_font);
            m_title.setString("SOUND SETTINGS");
            m_title.setCharacterSize(36);
            m_title.setFillColor(sf::Color::Yellow);
            CenterText(m_title, SCREEN_HEIGHT / 2.0f - 100);

            // Музыка
            m_musicText.setFont(m_font);
            m_musicText.setCharacterSize(30);
            CenterText(m_musicText, SCREEN_HEIGHT / 2.0f - 30);

            // Звуки
            m_soundsText.setFont(m_font);
            m_soundsText.setCharacterSize(30);
            CenterText(m_soundsText, SCREEN_HEIGHT / 2.0f + 20);

            // Подсказка
            m_hint.setFont(m_font);
            m_hint.setString("↑↓ - Navigate | Enter - Toggle | ESC - Close");
            m_hint.setCharacterSize(18);
            m_hint.setFillColor(sf::Color(150, 150, 150));
            CenterText(m_hint, SCREEN_HEIGHT / 2.0f + 100);

            // Confirmation текст
            m_confirmText.setFont(m_font);
            m_confirmText.setString("Disable? (Y/N)");
            m_confirmText.setCharacterSize(28);
            m_confirmText.setFillColor(sf::Color::Yellow);
            CenterText(m_confirmText, SCREEN_HEIGHT / 2.0f + 50);

            UpdateTexts();
        }

        void CenterText(sf::Text& text, float y)
        {
            if (text.getFont() == nullptr) return;
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
            text.setPosition(SCREEN_WIDTH / 2.0f, y);
        }

        void UpdateTexts()
        {
            // Обновляем цвета и текст
            m_musicText.setString("MUSIC: " + std::string(m_musicEnabled ? "ON" : "OFF"));
            m_musicText.setFillColor(m_selectedItem == 0 ? sf::Color::Yellow : sf::Color::White);

            m_soundsText.setString("SOUNDS: " + std::string(m_soundsEnabled ? "ON" : "OFF"));
            m_soundsText.setFillColor(m_selectedItem == 1 ? sf::Color::Yellow : sf::Color::White);
        }

        void Draw(sf::RenderWindow& window)
        {
            window.draw(m_background);
            window.draw(m_title);
            window.draw(m_musicText);
            window.draw(m_soundsText);

            if (m_waitingForConfirm)
                window.draw(m_confirmText);
            else
                window.draw(m_hint);
        }

        bool HandleInput(bool& musicEnabled, bool& soundsEnabled)
        {
            static bool upPressed = false, downPressed = false;
            static bool enterPressed = false, yPressed = false, nPressed = false;

            if (m_waitingForConfirm)
            {
                // Обработка Y/N
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && !yPressed)
                {
                    yPressed = true;
                    if (m_confirmItem == 0) musicEnabled = !musicEnabled;
                    else soundsEnabled = !soundsEnabled;
                    m_waitingForConfirm = false;
                    m_musicEnabled = musicEnabled;
                    m_soundsEnabled = soundsEnabled;
                    UpdateTexts();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && !nPressed)
                {
                    nPressed = true;
                    m_waitingForConfirm = false;
                }
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) yPressed = false;
                else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::N)) nPressed = false;

                return true;  // Попап активен
            }

            // Навигация
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !upPressed)
            {
                upPressed = true;
                m_selectedItem = (m_selectedItem - 1 + 2) % 2;
                UpdateTexts();
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                upPressed = false;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !downPressed)
            {
                downPressed = true;
                m_selectedItem = (m_selectedItem + 1) % 2;
                UpdateTexts();
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                downPressed = false;

            // Выбор пункта
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !enterPressed)
            {
                enterPressed = true;

                bool currentState = (m_selectedItem == 0) ? musicEnabled : soundsEnabled;
                if (currentState)
                {
                    // Если включено - спрашиваем подтверждение
                    m_waitingForConfirm = true;
                    m_confirmItem = m_selectedItem;
                }
                else
                {
                    // Если выключено - просто включаем
                    if (m_selectedItem == 0) musicEnabled = true;
                    else soundsEnabled = true;
                }

                m_musicEnabled = musicEnabled;
                m_soundsEnabled = soundsEnabled;
                UpdateTexts();
            }
            else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                enterPressed = false;

            return true;  // Попап активен
        }

        void Reset()
        {
            m_waitingForConfirm = false;
            m_selectedItem = 0;
        }

        void UpdateStates(bool musicEnabled, bool soundsEnabled)
        {
            m_musicEnabled = musicEnabled;
            m_soundsEnabled = soundsEnabled;
            UpdateTexts();
        }
    };
}
