#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include "Observer.h"

namespace RoqaliqueGame
{
    class Ball;

    class Block : public GameObject, public Colladiable
    {
    public:
        Block(const sf::Vector2f& position);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
        virtual void OnHit() override;

        bool IsActive() const { return active_; }
        virtual bool CheckCollision(Ball& ball);
        virtual int GetScoreValue() const { return 10; }

        sf::FloatRect GetCollisionRect() const;

        GameEventDispatcher onDestroyed;

    protected:
        bool active_ = true;
    };
}