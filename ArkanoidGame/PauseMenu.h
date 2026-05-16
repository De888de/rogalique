#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace ArkanoidGame
{
    class PauseMenu
    {
    public:
        PauseMenu();

        void Open();
        void Close();
        bool IsOpen() const { return isOpen; }

        void Draw(sf::RenderWindow& window);
        void HandleInput(const sf::Event& event);

        void SetOnResume(std::function<void()> callback) { onResume = callback; }
        void SetOnSaveAndExit(std::function<void()> callback) { onSaveAndExit = callback; }
        void SetOnRestartLevel(std::function<void()> callback) { onRestartLevel = callback; }

    private:
        void InitText();
        void MoveUp();
        void MoveDown();
        void UpdateSelection();
        void ExecuteCurrent();

        sf::Font font;
        sf::Text titleText;
        sf::Text resumeText;
        sf::Text saveExitText;
        sf::Text restartText;

        std::function<void()> onResume;
        std::function<void()> onSaveAndExit;
        std::function<void()> onRestartLevel;

        bool isOpen = false;
        int selectedIndex = 0;
        const int totalOptions = 3;
    };
}

