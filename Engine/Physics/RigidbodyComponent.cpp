#include "RigidbodyComponent.h"
#include "Colliders/Collider.h"
#include "../Core/TransformComponent.h"
#include "../Core/GameObject.h"
#include "Vector2.h"
#include <SFML/System/Vector2.hpp>

namespace engine {

    RigidbodyComponent::RigidbodyComponent() {
        UpdateInverseValues();
    }

    void RigidbodyComponent::UpdateInverseValues() {
        if (mass > 0.0f && !isKinematic) {
            invMass = 1.0f / mass;
            invInertia = 1.0f / inertia;
        }
        else {
            invMass = 0.0f;
            invInertia = 0.0f;
        }
    }

    void RigidbodyComponent::SetMass(float newMass) {
        mass = newMass;
        if (collider) inertia = mass * 1.0f;
        UpdateInverseValues();
    }

    TransformComponent* RigidbodyComponent::GetTransform() const {
        return gameObject ? gameObject->GetComponent<TransformComponent>() : nullptr;
    }

    void RigidbodyComponent::ApplyForce(Vector2 force) {
        if (isKinematic) return;
        forceAccumulator += force;
    }

    void RigidbodyComponent::ApplyImpulse(Vector2 impulse) {
        if (isKinematic) return;
        linearVelocity += impulse * invMass;
    }

    void RigidbodyComponent::ApplyForces() {
        if (isKinematic) return;
        linearVelocity += forceAccumulator * invMass;
        angularVelocity += torqueAccumulator * invInertia;
    }

    void RigidbodyComponent::Integrate(float deltaTime) {
        if (isKinematic) return;

        float linearDampingFactor = 1.0f - linearDamping * deltaTime;
        linearVelocity *= linearDampingFactor;
        angularVelocity *= (1.0f - angularDamping * deltaTime);

        auto* transform = GetTransform();
        if (transform) {
            // Получаем sf::Vector2f и конвертируем в engine::Vector2
            sf::Vector2f sfPos = transform->GetPosition();
            Vector2 currentPos(sfPos.x, sfPos.y);

            Vector2 deltaMove = linearVelocity * deltaTime;

            // Новое положение
            Vector2 newPos = currentPos + deltaMove;

            // Конвертируем обратно в sf::Vector2f
            transform->SetPosition(sf::Vector2f(newPos.x, newPos.y));
        }
    }

    void RigidbodyComponent::UpdatePhysics(float deltaTime) {
        if (!gameObject) return;
        ApplyForces();
        Integrate(deltaTime);
        ClearForces();
    }

    void RigidbodyComponent::ClearForces() {
        forceAccumulator = Vector2::Zero;
        torqueAccumulator = 0.0f;
    }

} 