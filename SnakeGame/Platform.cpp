#include "Platform.h"
#include "Ball.h"
#include "GameSettings.h"
#include <algorithm>
#include <iostream>

namespace ArkanoidGame
{
    Platform::Platform(const sf::Vector2f& position)
        : GameObject(SETTINGS.TEXTURES_PATH + std::string("platform.png"),
            position,
            SETTINGS.PLATFORM_WIDTH,
            SETTINGS.PLATFORM_HEIGHT)
    {
        std::cout << "[Platform] Loading from: " << SETTINGS.TEXTURES_PATH << "platform.png" << std::endl;

        if (sprite.getTexture() != nullptr)
        {
            std::cout << "[Platform] SUCCESS: platform.png loaded!" << std::endl;
        }
        else
        {
            std::cout << "[Platform] FAILED to load platform.png" << std::endl;
        }
    }

    void Platform::Update(float timeDelta)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            Move(-timeDelta * SETTINGS.PLATFORM_SPEED);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            Move(timeDelta * SETTINGS.PLATFORM_SPEED);
    }

    void Platform::Move(float speed)
    {
        auto pos = sprite.getPosition();
        pos.x = std::clamp(pos.x + speed,
            SETTINGS.PLATFORM_WIDTH / 2.0f,
            SETTINGS.SCREEN_WIDTH - SETTINGS.PLATFORM_WIDTH / 2.0f);
        sprite.setPosition(pos);
    }

    void Platform::Draw(sf::RenderWindow& window)
    {
        if (sprite.getTexture() != nullptr)
        {
            sprite.setColor(sf::Color::White);
            window.draw(sprite);
        }
        else
        {
            // Красный fallback
            sf::RectangleShape rect(sf::Vector2f(SETTINGS.PLATFORM_WIDTH, SETTINGS.PLATFORM_HEIGHT));
            rect.setOrigin(SETTINGS.PLATFORM_WIDTH / 2.0f, SETTINGS.PLATFORM_HEIGHT / 2.0f);
            rect.setPosition(GetPosition());
            rect.setFillColor(sf::Color::Red);
            rect.setOutlineColor(sf::Color::Yellow);
            rect.setOutlineThickness(4.0f);
            window.draw(rect);
        }
    }


    bool Platform::GetCollision(std::shared_ptr<Colladiable> collidable) const
    {
        auto other = std::dynamic_pointer_cast<GameObject>(collidable);
        if (!other) return false;
        return GetRect().intersects(other->GetRect());
    }

    void Platform::OnHit() {}

    bool Platform::CheckCollision(Ball& ball)
    {
        if (GetRect().intersects(ball.GetRect()))
        {
            auto vel = ball.GetVelocity();
            vel.y = -std::abs(vel.y);

            float hitOffset = (ball.GetPosition().x - GetPosition().x) / (SETTINGS.PLATFORM_WIDTH / 2.0f);
            vel.x += hitOffset * 180.0f;

            ball.SetVelocity(vel);
            return true;
        }
        return false;
    }
}