#pragma once
#include "Block.h"

namespace RoqaliqueGame
{
    class GlassBlock : public Block
    {
    public:
        GlassBlock(const sf::Vector2f& position);

        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball) override;
        void OnHit() override;
        int GetScoreValue() const override { return 20; }  // стеклянный даёт больше очков

        bool IsDestroyed() const { return isDestroyed; }

    private:
        bool isDestroyed = false;
        sf::Color glassColor = sf::Color(173, 216, 230);  // светло-голубой
        sf::Color outlineColor = sf::Color(0, 191, 255);  // ярко-голубая обводка
    };
}

