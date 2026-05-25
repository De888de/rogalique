#include "HealthComponent.h"
#include <algorithm>

namespace rogalique
{
    HealthComponent::HealthComponent(RogaliqueGameObject* owner, int maxHealth)
        : Component(owner), m_health(maxHealth), m_maxHealth(maxHealth) {}
    
    void HealthComponent::Update(float) {}
    void HealthComponent::Render(sf::RenderWindow&) {}
    
    void HealthComponent::TakeDamage(int damage)
    {
        m_health = std::max(0, m_health - damage);
    }
    
    void HealthComponent::Heal(int amount)
    {
        m_health = std::min(m_maxHealth, m_health + amount);
    }
}
