#pragma once
#include "GameObject.h"
#include "Collidable.h"

namespace RoqaliqueGame
{
    class Ball : public GameObject, public Colladiable
    {
    private:
        sf::Vector2f velocity;
        float speed;
        bool hasFallenThisFrame = false;

    public:
        Ball(const sf::Vector2f& position);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool GetCollision(std::shared_ptr<Colladiable> collidableObject) const override;
        void OnHit() override {}

        void SetVelocity(const sf::Vector2f& newVelocity);
        sf::Vector2f GetVelocity() const;
        void Reset();

        sf::Vector2f GetPosition() const;

        bool HasFallen() const;
        void ClearFallenFlag();

        

        void BounceX();
        void BounceY();
    };
}