#pragma once
#include "Block.h"
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <memory>

namespace ArkanoidGame
{
    class MagicBlock : public Block
    {
    public:
        MagicBlock(const sf::Vector2f& position);

        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball) override;
        void OnHit() override;
        int GetScoreValue() const override;

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