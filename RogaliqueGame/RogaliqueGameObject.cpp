#include "RogaliqueGameObject.h"

namespace rogalique
{
    RogaliqueGameObject::RogaliqueGameObject()
        : m_position(0, 0)
        , m_isActive(true)
    {}

    void RogaliqueGameObject::Draw(sf::RenderWindow& window)
    {
        // Пустая реализация — наследники переопределят
    }

    sf::FloatRect RogaliqueGameObject::GetBounds() const
    {
        return sf::FloatRect(m_position.x, m_position.y, 0, 0);
    }
}
