#pragma once
#include "Ball.h"
#include <memory>
#include <iostream>

namespace ArkanoidGame
{
    // Базовый декоратор для мяча
    class BallDecorator : public Ball
    {
    protected:
        std::unique_ptr<Ball> wrappedBall;
        float duration = 0.0f;
        float maxDuration = 7.0f;

    public:
        BallDecorator(std::unique_ptr<Ball> ball, float durationSec)
            : Ball(ball->GetPosition())
            , wrappedBall(std::move(ball))
            , duration(durationSec)
            , maxDuration(durationSec)
        {
            // Используем геттеры/сеттеры вместо прямого доступа
            SetVelocity(wrappedBall->GetVelocity());
        }

        void Update(float timeDelta) override
        {
            duration -= timeDelta;
            if (duration > 0)
            {
                wrappedBall->SetPosition(GetPosition());
                wrappedBall->SetVelocity(GetVelocity());
                wrappedBall->Update(timeDelta);
                SetPosition(wrappedBall->GetPosition());
                SetVelocity(wrappedBall->GetVelocity());
            }
            else
            {
                wrappedBall->Update(timeDelta);
                SetPosition(wrappedBall->GetPosition());
                SetVelocity(wrappedBall->GetVelocity());
            }
        }

        void Draw(sf::RenderWindow& window) override
        {
            wrappedBall->Draw(window);
        }

        bool IsActive() const { return duration > 0; }
        float GetRemainingTime() const { return duration; }
    };

    // Огненный шар - пробивает блоки
    class FireBallDecorator : public BallDecorator
    {
    public:
        FireBallDecorator(std::unique_ptr<Ball> ball, float durationSec)
            : BallDecorator(std::move(ball), durationSec)
        {
            // Используем сеттер вместо прямого доступа
            sf::Vector2f vel = GetVelocity();
            vel.x *= 1.5f;
            vel.y *= 1.5f;
            SetVelocity(vel);
            wrappedBall->SetVelocity(vel);
            std::cout << "[FireBall] Activated! Speed increased!" << std::endl;
        }

        bool CheckBlockCollision(class Block& block)
        {
            return true;
        }
    };

    // Обнуляющий шар - всё с одного удара
    class DestroyAllBallDecorator : public BallDecorator
    {
    public:
        DestroyAllBallDecorator(std::unique_ptr<Ball> ball, float durationSec)
            : BallDecorator(std::move(ball), durationSec)
        {
            std::cout << "[DestroyAllBall] Activated! One hit kill!" << std::endl;
        }
    };
}