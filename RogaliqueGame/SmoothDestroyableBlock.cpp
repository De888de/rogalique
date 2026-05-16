#include "SmoothDestroyableBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>

namespace RoqaliqueGame
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

    bool SmoothDestroyableBlock::CheckCollision(Ball& ball)
    {
        if (!active_) return false;

        sf::FloatRect ballRect = ball.GetRect();
        sf::FloatRect blockRect = GetCollisionRect();
        if (!ballRect.intersects(blockRect))
            return false;

        // Выталкивание мяча
        sf::Vector2f ballPos = ball.GetPosition();
        sf::Vector2f correction(0.f, 0.f);

        float leftOverlap = ballRect.left + ballRect.width - blockRect.left;
        float rightOverlap = blockRect.left + blockRect.width - ballRect.left;
        float topOverlap = ballRect.top + ballRect.height - blockRect.top;
        float bottomOverlap = blockRect.top + blockRect.height - ballRect.top;

        float minX = std::min(leftOverlap, rightOverlap);
        float minY = std::min(topOverlap, bottomOverlap);

        if (minX < minY)
        {
            if (leftOverlap < rightOverlap)
                correction.x = -leftOverlap;
            else
                correction.x = rightOverlap;
            ball.BounceX();
        }
        else
        {
            if (topOverlap < bottomOverlap)
                correction.y = -topOverlap;
            else
                correction.y = bottomOverlap;
            ball.BounceY();
        }
        ball.SetPosition(ballPos + correction);

        // Эффект от удара
        sf::Vector2f vel = ball.GetVelocity();
        float hitOffset = (ball.GetPosition().x - GetPosition().x) / (SETTINGS.BLOCK_WIDTH / 2.0f);
        vel.x += hitOffset * 70.0f;

        float currentSpeed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
        if (currentSpeed > 0.0f)
        {
            float targetSpeed = SETTINGS.BALL_SPEED;
            vel.x = (vel.x / currentSpeed) * targetSpeed;
            vel.y = (vel.y / currentSpeed) * targetSpeed;
        }
        ball.SetVelocity(vel);

        OnHit();   // запускаем анимацию и начисляем очки
        return true;
    }

    void SmoothDestroyableBlock::OnHit()
    {
        if (!isDying && active_)
        {
            // Начисляем очки и говорим, что блок разрушен (для счётчика)
            Block::OnHit();
            isDying = true;
            deathTimer = 0.0f;
            currentColor = sf::Color::White;
            // active_ не меняем, чтобы анимация проигрывалась
        }
    }
}