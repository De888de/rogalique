#include "Player.h"
#include <algorithm>

namespace rogalique
{
    Player::Player()
    {
        // Пока пусто, компоненты добавим позже
    }
    
    void Player::TakeDamage(int damage)
    {
        m_health = std::max(0, m_health - damage);
    }
    
    void Player::Heal(int amount)
    {
        m_health = std::min(m_maxHealth, m_health + amount);
    }
}
