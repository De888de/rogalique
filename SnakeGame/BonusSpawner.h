#pragma once
#include <SFML/Graphics.hpp>
#include "Bonus.h"
#include "Ball.h"
#include "Platform.h"
#include "GameSettings.h"
#include <vector>
#include <random>
#include <memory>
#include <chrono>
#include <functional>
#include <algorithm>

namespace ArkanoidGame
{
    class BonusSpawner
    {
    private:
        std::vector<std::unique_ptr<Bonus>> activeBonuses;
        std::mt19937 rng;
        std::uniform_int_distribution<int> chanceDist;
        float dropChance = 10.0f;

    public:
        BonusSpawner()
            : rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
            , chanceDist(1, 100)
        {
        }

        void TrySpawnBonus(const sf::Vector2f& blockPosition)
        {
            int chance = chanceDist(rng);
            if (chance <= dropChance)
            {
                std::uniform_int_distribution<int> typeDist(0, 2);
                int type = typeDist(rng);

                BonusType bonusType;
                switch (type)
                {
                case 0: bonusType = BonusType::FireBall; break;
                case 1: bonusType = BonusType::DestroyAll; break;
                default: bonusType = BonusType::BigPlatform; break;
                }

                auto bonus = std::make_unique<Bonus>(bonusType, blockPosition);
                activeBonuses.push_back(std::move(bonus));
                std::cout << "[BonusSpawner] Bonus spawned! Type: " << type << std::endl;
            }
        }

        void Update(float timeDelta)
        {
            for (auto& bonus : activeBonuses)
            {
                bonus->Update(timeDelta);
            }

            activeBonuses.erase(
                std::remove_if(activeBonuses.begin(), activeBonuses.end(),
                    [](const std::unique_ptr<Bonus>& bonus) {
                        return !bonus->IsActive() || bonus->GetPosition().y > SETTINGS.SCREEN_HEIGHT + 100;
                    }),
                activeBonuses.end()
            );
        }

        void Draw(sf::RenderWindow& window)
        {
            for (auto& bonus : activeBonuses)
            {
                bonus->Draw(window);
            }
        }

        void CheckCollisionWithPlatform(const sf::FloatRect& platformRect,
            std::unique_ptr<Ball>& ball,
            std::unique_ptr<Platform>& platform,
            std::function<void(BonusType)> onBonusCollected)
        {
            for (auto& bonus : activeBonuses)
            {
                if (bonus->IsActive() && bonus->GetRect().intersects(platformRect))
                {
                    onBonusCollected(bonus->GetType());
                    bonus->Deactivate();
                    std::cout << "[BonusSpawner] Bonus collected!" << std::endl;
                }
            }
        }

        void Clear()
        {
            activeBonuses.clear();
        }
    };
}
