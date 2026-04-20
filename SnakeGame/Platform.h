#pragma once
#include "GameObject.h"
#include "Collidable.h"

namespace ArkanoidGame
{
    class Ball;   // forward declaration

    class Platform : public GameObject, public Colladiable
    {
    public:
        Platform(const sf::Vector2f& position);

        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool GetCollision(std::shared_ptr<Colladiable> collidable) const override;
        void OnHit() override;


        bool CheckCollision(Ball& ball);

    private:
        void Move(float speed);
    };
}