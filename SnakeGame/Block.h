#pragma once
#include "GameObject.h"
#include "Collidable.h"

namespace ArkanoidGame
{
    class Ball;

    class Block : public GameObject, public Colladiable
    {
    public:
        Block(const sf::Vector2f& position);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
        void OnHit() override;

        bool IsActive() const { return active_; }
        bool CheckCollision(Ball& ball);

        // Прямоугольник коллизии (на основе центра и размера)
        sf::FloatRect GetCollisionRect() const;

    private:
        bool active_ = true;
    };
}
