#include "TransformMatrix.h"
#include <cmath>

namespace rogalique {

TransformMatrix::TransformMatrix() {
    Reset();
}

TransformMatrix::~TransformMatrix() {}

void TransformMatrix::UpdateMatrix() {
    m_transform = sf::Transform::Identity;
    m_transform.translate(m_position);
    m_transform.rotate(m_rotation);
    m_transform.scale(m_scale.x, m_scale.y);
}

void TransformMatrix::SetPosition(const sf::Vector2f& pos) {
    m_position = pos;
    UpdateMatrix();
}

void TransformMatrix::SetPosition(float x, float y) {
    SetPosition(sf::Vector2f(x, y));
}

sf::Vector2f TransformMatrix::GetPosition() const {
    return m_position;
}

void TransformMatrix::SetScale(const sf::Vector2f& scale) {
    m_scale = scale;
    UpdateMatrix();
}

void TransformMatrix::SetScale(float x, float y) {
    SetScale(sf::Vector2f(x, y));
}

sf::Vector2f TransformMatrix::GetScale() const {
    return m_scale;
}

void TransformMatrix::SetRotation(float angleDeg) {
    m_rotation = angleDeg;
    UpdateMatrix();
}

float TransformMatrix::GetRotation() const {
    return m_rotation;
}

sf::Vector2f TransformMatrix::TransformPoint(const sf::Vector2f& point) const {
    return m_transform.transformPoint(point);
}

sf::Transform TransformMatrix::GetSFTransform() const {
    return m_transform;
}

void TransformMatrix::Reset() {
    m_position = {0, 0};
    m_scale = {1, 1};
    m_rotation = 0;
    UpdateMatrix();
}

void TransformMatrix::Combine(const TransformMatrix& other) {
    m_transform.combine(other.m_transform);
}

} // namespace rogalique
