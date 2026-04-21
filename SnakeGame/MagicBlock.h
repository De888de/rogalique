#pragma once
#include "Block.h"
#include <SFML/Graphics/Text.hpp>
#include <vector>

namespace ArkanoidGame
{
    class MagicBlock : public Block  // ← Block уже реализует Colladiable
    {
    public:
        MagicBlock(const sf::Vector2f& position);

        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball);  // ← свой метод для логики с мячом
        void OnHit() override;             // ← переопределяем из Colladiable
        int GetScoreValue() const override { return 1000; }

        sf::Vector2f GetBlockPosition() const { return GetPosition(); }

    private:
        int hitsRemaining = 5;
        sf::Font font;
        sf::Text hitCounterText;

        void UpdateHitCounterText();
        void ExplodeNeighbors();
    };

    void SetGlobalBlocksPtr(std::vector<std::unique_ptr<Block>>* blocks);
}

