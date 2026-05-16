#pragma once
#include "Block.h"

namespace ArkanoidGame
{
    class IndestructibleBlock : public Block
    {
    public:
        IndestructibleBlock(const sf::Vector2f& position);

        void Draw(sf::RenderWindow& window) override;
        bool CheckCollision(Ball& ball) override;
        void OnHit() override;
        int GetScoreValue() const override { return 0; }  // не даёт очков

        bool IsDestroyable() const { return false; }  // флаг, что блок неуничтожимый

    private:
        sf::Color blockColor = sf::Color(101, 67, 33);  // тёмно-коричневый (RGB)
    };
}

