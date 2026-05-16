#include "Ball.h"
#include "GameSettings.h"
#include "Platform.h"
#include <cmath>

namespace RoqaliqueGame
{
    Ball::Ball(const sf::Vector2f& position)
        : GameObject(SETTINGS.TEXTURES_PATH + std::string("ball.png"), position,
            SETTINGS.BALL_SIZE, SETTINGS.BALL_SIZE)
        , speed(SETTINGS.BALL_SPEED)
    {
        velocity = { speed * 0.7f, -speed * 0.7f };
        hasFallenThisFrame = false;
    }

    void Ball::Update(float timeDelta)
    {
        sprite.move(velocity * timeDelta);

        auto pos = sprite.getPosition();
        auto bounds = sprite.getGlobalBounds();

        if (pos.x <= 0 || pos.x + bounds.width >= SETTINGS.SCREEN_WIDTH)
            BounceX();

        if (pos.y <= 0)
            BounceY();

        // Шар упал вниз
        if (pos.y > SETTINGS.SCREEN_HEIGHT + 50.0f && !hasFallenThisFrame)
        {
            hasFallenThisFrame = true;
            // Убираем лишний вывод
            // std::cout << "[Ball] Has fallen!" << std::endl;
        }
    }

    bool Ball::HasFallen() const
    {
        return hasFallenThisFrame;
    }

    void Ball::ClearFallenFlag()
    {
        hasFallenThisFrame = false;
        Reset();  // Сбрасываем позицию при очистке флага
    }

    void Ball::Draw(sf::RenderWindow& window)
    {
        GameObject::Draw(window);
    }

    bool Ball::GetCollision(std::shared_ptr<Colladiable> collidableObject) const
    {
        auto other = std::dynamic_pointer_cast<GameObject>(collidableObject);
        if (!other) return false;
        return GetRect().intersects(other->GetRect());
    }

    void Ball::BounceX() { velocity.x = -velocity.x; }
    void Ball::BounceY() { velocity.y = -velocity.y; }

    void Ball::SetVelocity(const sf::Vector2f& newVelocity) { velocity = newVelocity; }
    sf::Vector2f Ball::GetVelocity() const { return velocity; }

    void Ball::Reset()
    {
        sprite.setPosition(SETTINGS.SCREEN_WIDTH / 2.f, SETTINGS.SCREEN_HEIGHT / 2.f);
        velocity = { speed * 0.7f, -speed * 0.7f };
    }

    sf::Vector2f Ball::GetPosition() const { return sprite.getPosition(); }
}