#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>

namespace rogalique {

class TransformMatrix {
public:
    TransformMatrix();
    ~TransformMatrix();
    
    // Управление позицией
    void SetPosition(const sf::Vector2f& pos);
    void SetPosition(float x, float y);
    sf::Vector2f GetPosition() const;
    
    // Управление масштабом
    void SetScale(const sf::Vector2f& scale);
    void SetScale(float x, float y);
    sf::Vector2f GetScale() const;
    
    // Управление поворотом
    void SetRotation(float angleDeg);
    float GetRotation() const;
    
    // Трансформация точки
    sf::Vector2f TransformPoint(const sf::Vector2f& point) const;
    
    // Получение матрицы SFML
    sf::Transform GetSFTransform() const;
    
    // Сброс
    void Reset();
    
    // Комбинирование
    void Combine(const TransformMatrix& other);
    
private:
    sf::Transform m_transform;
    sf::Vector2f m_position = {0, 0};
    sf::Vector2f m_scale = {1, 1};
    float m_rotation = 0;
    
    void UpdateMatrix();
};

} // namespace rogalique
