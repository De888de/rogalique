#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <iostream>

namespace RoqaliqueGame
{
    enum class BonusType
    {
        FireBall,
        DestroyAll,
        BigPlatform
    };

    class Bonus
    {
    protected:
        sf::RectangleShape shape;
        sf::Vector2f position;
        sf::Vector2f velocity;
        BonusType type;
        bool active = true;
        float size = 20.0f;

    public:
        Bonus(BonusType bonusType, const sf::Vector2f& pos)
            : type(bonusType), position(pos)
        {
            velocity = { 0.0f, 150.0f };

            shape.setSize(sf::Vector2f(size, size));
            shape.setOrigin(size / 2, size / 2);
            shape.setPosition(position);

            switch (type)
            {
            case BonusType::FireBall:
                shape.setFillColor(sf::Color(255, 100, 0));
                break;
            case BonusType::DestroyAll:
                shape.setFillColor(sf::Color(255, 0, 255));
                break;
            case BonusType::BigPlatform:
                shape.setFillColor(sf::Color(0, 255, 255));
                break;
            }
            shape.setOutlineColor(sf::Color::White);
            shape.setOutlineThickness(2);
        }

        virtual ~Bonus() = default;

        void Update(float timeDelta)
        {
            position += velocity * timeDelta;
            shape.setPosition(position);
        }

        void Draw(sf::RenderWindow& window)
        {
            if (active)
                window.draw(shape);
        }

        sf::FloatRect GetRect() const
        {
            return shape.getGlobalBounds();
        }

        bool IsActive() const { return active; }
        void Deactivate() { active = false; }
        BonusType GetType() const { return type; }
        sf::Vector2f GetPosition() const { return position; }
    };
}