#pragma once
#include "Component.h"
#include "TransformMatrix.h"

namespace rogalique
{
    class TransformComponent : public Component
    {
    public:
        TransformComponent(RogaliqueGameObject* owner);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        void SetPosition(const sf::Vector2f& pos);
        sf::Vector2f GetPosition() const;

        void SetRotation(float angleDeg);
        void SetScale(float x, float y);

        // Получение итоговой матрицы трансформации
        const TransformMatrix& GetWorldTransform() const;

    private:
        void UpdateWorldTransform();

        sf::Vector2f m_position;
        TransformMatrix m_worldTransform;
        bool m_isDirty = true;
        float m_rotation = 0.0f;
        sf::Vector2f m_scale{1.0f, 1.0f};
    };
}
