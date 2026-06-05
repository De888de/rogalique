#include "Engine/Core/GameObject.h"
#include "Engine/Core/TransformComponent.h"
#include "Engine/Physics/RigidbodyComponent.h"
#include "Engine/Physics/Colliders/CircleCollider.h"
#include "Engine/Physics/World/PhysicsWorld.h"
#include <iostream>

using namespace engine;

int main() {
    PhysicsWorld world;
    world.SetGravity(sf::Vector2f(0, -9.81f));
    
    GameObject ball;
    auto* transform = ball.AddComponent<TransformComponent>();
    transform->position = sf::Vector2f(0, 5);
    
    auto* collider = ball.AddComponent<CircleCollider>(0.5f);
    auto* rigidbody = ball.AddComponent<RigidbodyComponent>();
    rigidbody->SetMass(1.0f);
    rigidbody->AttachCollider(collider);
    
    world.AddRigidbody(rigidbody);
    
    for (int i = 0; i < 60; i++) {
        world.Update(1.0f / 60.0f);
        std::cout << "Ball Y: " << transform->position.y << std::endl;
        if (transform->position.y <= 0) break;
    }
    
    std::cout << "Physics test complete!" << std::endl;
    return 0;
}
