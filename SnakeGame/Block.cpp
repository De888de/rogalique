#include "Block.h"
#include "Ball.h"
#include "GameSettings.h"
#include <cmath>

namespace ArkanoidGame
{
    Block::Block(const sf::Vector2f& position)
        : GameObject(SETTINGS.TEXTURES_PATH + std::string("block.png"),
            position,
            SETTINGS.BLOCK_WIDTH,
            SETTINGS.BLOCK_HEIGHT)
    {
    }

    void Block::Update(float timeDelta)
    {
    }

    void Block::Draw(sf::RenderWindow& window)
    {
        if (!active_) return;

        sf::RectangleShape rect(sf::Vector2f(SETTINGS.BLOCK_WIDTH, SETTINGS.BLOCK_HEIGHT));
        rect.setOrigin(SETTINGS.BLOCK_WIDTH / 2.f, SETTINGS.BLOCK_HEIGHT / 2.f);
        rect.setPosition(GetPosition());
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Red);
        rect.setOutlineThickness(2);
        window.draw(rect);
    }

    sf::FloatRect Block::GetCollisionRect() const
    {
        sf::Vector2f center = GetPosition();
        return sf::FloatRect(
            center.x - SETTINGS.BLOCK_WIDTH / 2.f,
            center.y - SETTINGS.BLOCK_HEIGHT / 2.f,
            SETTINGS.BLOCK_WIDTH,
            SETTINGS.BLOCK_HEIGHT
        );
    }

    bool Block::GetCollision(std::shared_ptr<Colladiable> collidable) const
    {
        if (!active_) return false;
        auto other = std::dynamic_pointer_cast<GameObject>(collidable);
        if (!other) return false;
        return GetCollisionRect().intersects(other->GetRect());
    }

    void Block::OnHit()
    {
        active_ = false;
    }

    bool Block::CheckCollision(Ball& ball)
    {
        if (!active_) return false;

        sf::FloatRect ballRect = ball.GetRect();
        sf::FloatRect blockRect = GetCollisionRect();
        if (!ballRect.intersects(blockRect))
            return false;

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

        OnHit();
        return true;
    }
}