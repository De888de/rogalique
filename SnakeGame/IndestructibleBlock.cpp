#include "IndestructibleBlock.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>
#include <algorithm>

namespace ArkanoidGame
{
    IndestructibleBlock::IndestructibleBlock(const sf::Vector2f& position)
        : Block(position)
    {
        active_ = true;  // всегда активен
    }

    void IndestructibleBlock::Draw(sf::RenderWindow& window)
    {
        if (!active_) return;

        // Рисуем тёмно-коричневый блок
        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(blockColor);
        rect.setOutlineColor(sf::Color(80, 50, 25));  // тёмная обводка
        rect.setOutlineThickness(2);
        window.draw(rect);

       
    }

    bool IndestructibleBlock::CheckCollision(Ball& ball)
    {
        if (!active_) return false;

        sf::FloatRect ballRect = ball.GetRect();
        sf::FloatRect blockRect = GetCollisionRect();
        if (!ballRect.intersects(blockRect))
            return false;

        // Выталкивание мяча (как в обычном блоке)
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

        

        return false;
    }

    void IndestructibleBlock::OnHit()
    {
     

    }
}
