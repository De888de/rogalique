#include "SmoothDestroyableBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>

namespace ArkanoidGame
{
    SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position)
        : Block(position)
    {
        currentColor = sf::Color::White;
    }

    void SmoothDestroyableBlock::Update(float timeDelta)
    {
        if (isDying)
        {
            deathTimer += timeDelta;

            float progress = deathTimer / deathDuration;
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1.0f - progress));
            currentColor.a = alpha;

            if (deathTimer >= deathDuration)
            {
                active_ = false;
                isDying = false;
            }
        }
    }

    void SmoothDestroyableBlock::Draw(sf::RenderWindow& window)
    {
        if (!active_ && !isDying) return;

        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());

        if (isDying)
        {
            rect.setFillColor(currentColor);
            rect.setOutlineColor(sf::Color(currentColor.r, currentColor.g, currentColor.b, currentColor.a));
        }
        else
        {
            rect.setFillColor(sf::Color::White);
            rect.setOutlineColor(sf::Color::Red);
            rect.setOutlineThickness(2);
        }

        window.draw(rect);
    }

    void SmoothDestroyableBlock::OnHit()
    {
        if (!isDying && active_)
        {
            isDying = true;
            deathTimer = 0.0f;
            currentColor = sf::Color::White;
        }
    }
}