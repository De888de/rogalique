#pragma once
#include "Component.h"
#include "RogaliqueGameObject.h"
#include <cmath>

namespace rogalique
{
    class SeekerComponent : public Component
    {
    public:
        SeekerComponent(RogaliqueGameObject* owner, float speed);
        void Update(float deltaTime) override;
        void Render(sf::RenderWindow& window) override;

        void SetTarget(RogaliqueGameObject* target);
        RogaliqueGameObject* GetTarget() const { return m_target; }

    private:
        RogaliqueGameObject* m_target = nullptr;
        float m_speed;
    };
}
