#pragma once
#include "RogaliqueGameObject.h"
#include <SFML/Graphics/CircleShape.hpp>

namespace rogalique {

class IsometricPhysicsItem : public RogaliqueGameObject {
public:
    IsometricPhysicsItem();
    void Spawn(float worldX, float worldZ);
    void Update(float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    
    void ApplyImpulse(float velX, float velZ, float velY);
    
private:
    // Мировые координаты (X, Z — карта, Y — высота)
    sf::Vector2f m_worldPos;    // X, Z на карте
    float m_height = 0.0f;       // высота над землёй
    
    // Скорости
    sf::Vector2f m_velGround;    // скорость по земле (X, Z)
    float m_velY = 0.0f;         // вертикальная скорость
    
    // Параметры физики
    float m_gravity = 800.0f;    // сила гравитации
    float m_bounciness = 0.6f;   // упругость отскока
    float m_damping = 0.98f;      // сопротивление воздуха
    float m_radius = 12.0f;       // радиус предмета
    
    // Для отрисовки
    sf::CircleShape m_shape;
    sf::CircleShape m_shadow;     // тень на земле
    bool m_active = true;
    
    // Вспомогательные функции
    sf::Vector2f WorldToScreen(const sf::Vector2f& world, float height) const;
};

} // namespace rogalique
