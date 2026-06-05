#include "RigidbodyComponent.h"
#include "Colliders/Collider.h"
#include "../Core/TransformComponent.h"
#include "../Core/GameObject.h"
#include <cmath>

namespace engine {

RigidbodyComponent::RigidbodyComponent() {
    UpdateInverseValues();
}

void RigidbodyComponent::UpdateInverseValues() {
    if (mass > 0 && !isKinematic) {
        invMass = 1.0f / mass;
        invInertia = 1.0f / inertia;
    } else {
        invMass = 0;
        invInertia = 0;
    }
}

void RigidbodyComponent::SetMass(float newMass) {
    mass = newMass;
    if (collider) {
        inertia = mass * 1.0f;
    }
    UpdateInverseValues();
}

TransformComponent* RigidbodyComponent::GetTransform() const {
    return gameObject ? gameObject->GetComponent<TransformComponent>() : nullptr;
}

void RigidbodyComponent::ApplyForce(sf::Vector2f force) {
    if (isKinematic) return;
    forceAccumulator.x += force.x;
    forceAccumulator.y += force.y;
}

void RigidbodyComponent::ApplyImpulse(sf::Vector2f impulse) {
    if (isKinematic) return;
    linearVelocity.x += impulse.x * invMass;
    linearVelocity.y += impulse.y * invMass;
}

void RigidbodyComponent::ApplyForces() {
    if (isKinematic) return;
    
    linearVelocity.x += forceAccumulator.x * invMass;
    linearVelocity.y += forceAccumulator.y * invMass;
    angularVelocity += torqueAccumulator * invInertia;
}

void RigidbodyComponent::Integrate(float deltaTime) {
    if (isKinematic) return;
    
    linearVelocity.x *= (1.0f - linearDamping * deltaTime);
    linearVelocity.y *= (1.0f - linearDamping * deltaTime);
    angularVelocity *= (1.0f - angularDamping * deltaTime);
    
    transform = GetTransform();
    if (transform) {
        transform->position.x += linearVelocity.x * deltaTime;
        transform->position.y += linearVelocity.y * deltaTime;
        transform->rotation += angularVelocity * deltaTime;
    }
}

void RigidbodyComponent::UpdatePhysics(float deltaTime) {
    if (!gameObject) return;
    
    transform = GetTransform();
    if (!transform) return;
    
    ApplyForces();
    Integrate(deltaTime);
    ClearForces();
}

void RigidbodyComponent::ClearForces() {
    forceAccumulator.x = 0;
    forceAccumulator.y = 0;
    torqueAccumulator = 0;
}

} // namespace engine
