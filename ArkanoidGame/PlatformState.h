#pragma once
#include "Platform.h"
#include <memory>

namespace ArkanoidGame
{
    // Состояния платформы
    class IPlatformState
    {
    public:
        virtual ~IPlatformState() = default;
        virtual void Update(Platform& platform, float timeDelta) = 0;
        virtual float GetWidthMultiplier() const = 0;
    };

    class NormalPlatformState : public IPlatformState
    {
    public:
        void Update(Platform& platform, float timeDelta) override {}
        float GetWidthMultiplier() const override { return 1.0f; }
    };

    class BigPlatformState : public IPlatformState
    {
    private:
        float remainingTime = 7.0f;

    public:
        BigPlatformState(float duration) : remainingTime(duration) {}

        void Update(Platform& platform, float timeDelta) override
        {
            remainingTime -= timeDelta;
            if (remainingTime <= 0)
            {
                // Возврат к нормальному состоянию будет обработан в Platform
            }
        }

        float GetWidthMultiplier() const override { return 2.0f; }
        float GetRemainingTime() const { return remainingTime; }
        bool IsExpired() const { return remainingTime <= 0; }
    };
}
