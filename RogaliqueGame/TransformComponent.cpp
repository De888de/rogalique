#include "TransformComponent.h"
#include "RogaliqueGameObject.h"

namespace rogalique
{
    TransformComponent::TransformComponent(RogaliqueGameObject* owner)
        : Component(owner), m_position(0, 0)
    {
    }

    void TransformComponent::Update(float deltaTime)
    {
        (void)deltaTime;
    }

    void TransformComponent::Render(sf::RenderWindow& window)
    {
        (void)window;
    }

    void TransformComponent::SetPosition(const sf::Vector2f& pos)
    {
        m_position = pos;
        m_isDirty = true;
    }

    sf::Vector2f TransformComponent::GetPosition() const
    {
        return m_position;
    }

    void TransformComponent::SetRotation(float angle)
    {
        m_rotation = angle;
        m_isDirty = true;
    }

    void TransformComponent::SetScale(float x, float y)
    {
        m_scale = {x, y};
        m_isDirty = true;
    }

    void TransformComponent::UpdateWorldTransform()
    {
        // TODO: обновление матрицы трансформации
        m_isDirty = false;
    }
}
