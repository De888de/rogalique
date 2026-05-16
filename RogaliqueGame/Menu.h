#pragma once
#include <SFML/Graphics.hpp>

namespace RoqaliqueGame {

    class Menu {
    public:
        Menu();

        void Update(float timeDelta);
        void Draw(sf::RenderWindow& window);
        void HandleWindowEvent(const sf::Event& event);

        bool HasSave() const;
        void UpdateHasSave(bool saveExists);

    private:
        void InitText();
        void MoveUp();
        void MoveDown();
        void UpdateSelection();
        void ExecuteCurrent();

        sf::Font font;
        sf::Text titleText;
        sf::Text newGameText;
        sf::Text continueText;
        sf::Text exitText;

        bool hasSave = false;
        int selectedIndex = 0;
        const int totalOptions = 3;  // NEW GAME, CONTINUE, EXIT
    };

} // namespace ArkanoidGame