#include "pch.h"
#include "TransformComponent.h"

namespace rogalique
{
    TransformComponent::TransformComponent(RogaliqueGameObject* owner)
        : Component(owner), m_position(0, 0)
    {
    }

    void TransformComponent::Update(float deltaTime)
    {
        // Пока ничего не обновляем
        (void)deltaTime;
    }

    void TransformComponent::Render(sf::RenderWindow& window)
    {
        // TransformComponent сам ничего не рисует
        (void)window;
    }

    void TransformComponent::SetPosition(const sf::Vector2f& pos)
    {
        m_position = pos;
    }

    sf::Vector2f TransformComponent::GetPosition() const
    {
        return m_position;
    }
}
