#pragma once
#include "Bonus.h"
#include "Ball.h"
#include "Platform.h"
#include "GameSettings.h"
#include <iostream>
#include <memory>
#include <cmath>

namespace ArkanoidGame
{
    // Интерфейс стратегии бонуса
    class IBonusStrategy
    {
    public:
        virtual ~IBonusStrategy() = default;
        virtual void ApplyEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) = 0;
        virtual void RemoveEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) = 0;
        virtual void Update(float timeDelta) {}
        virtual bool IsActive() const { return false; }
        virtual float GetRemainingTime() const { return 0.0f; }
    };

    // Стратегия огненного шара
    class FireBallStrategy : public IBonusStrategy
    {
    private:
        bool isActive = false;
        float duration = 0.0f;
        sf::Vector2f originalVelocity;

    public:
        void ApplyEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) override
        {
            if (!isActive && ball)
            {
                originalVelocity = ball->GetVelocity();
                sf::Vector2f newVelocity = originalVelocity;
                newVelocity.x *= 1.5f;
                newVelocity.y *= 1.5f;
                ball->SetVelocity(newVelocity);
                isActive = true;
                duration = 7.0f;
                std::cout << "[Bonus] FireBall activated! Speed increased!" << std::endl;
            }
        }

        void Update(float timeDelta) override
        {
            if (isActive)
            {
                duration -= timeDelta;
                if (duration <= 0)
                {
                    // Создаём временные переменные для вызова RemoveEffect
                    std::unique_ptr<Ball> emptyBall;
                    std::unique_ptr<Platform> emptyPlatform;
                    RemoveEffect(emptyBall, emptyPlatform);
                }
            }
        }

        void RemoveEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) override
        {
            if (isActive)
            {
                if (ball)
                {
                    ball->SetVelocity(originalVelocity);
                }
                isActive = false;
                std::cout << "[Bonus] FireBall expired!" << std::endl;
            }
        }

        bool IsActive() const override { return isActive; }
        float GetRemainingTime() const override { return duration; }
    };

    // Стратегия увеличенной платформы
    class BigPlatformStrategy : public IBonusStrategy
    {
    private:
        float originalWidth;
        float duration = 0.0f;
        bool isActive = false;

    public:
        BigPlatformStrategy() : originalWidth(SETTINGS.PLATFORM_WIDTH) {}

        void ApplyEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) override
        {
            if (!isActive && platform)
            {
                platform->GetSprite().setScale(2.0f, 1.0f);
                isActive = true;
                duration = 7.0f;
                std::cout << "[Bonus] BigPlatform activated! Width doubled!" << std::endl;
            }
        }

        void Update(float timeDelta) override
        {
            if (isActive)
            {
                duration -= timeDelta;
                if (duration <= 0)
                {
                    std::unique_ptr<Ball> emptyBall;
                    std::unique_ptr<Platform> emptyPlatform;
                    RemoveEffect(emptyBall, emptyPlatform);
                }
            }
        }

        void RemoveEffect(std::unique_ptr<Ball>& ball, std::unique_ptr<Platform>& platform) override
        {
            if (isActive)
            {
                if (platform)
                {
                    platform->GetSprite().setScale(1.0f, 1.0f);
                }
                isActive = false;
                std::cout << "[Bonus] BigPlatform expired!" << std::endl;
            }
        }

        bool IsActive() const override { return isActive; }
        float GetRemainingTime() const override { return duration; }
    };
}
