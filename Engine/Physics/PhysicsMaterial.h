#pragma once

namespace engine {

struct PhysicsMaterial {
    float density = 1.0f;
    float restitution = 0.5f;
    float staticFriction = 0.5f;
    float dynamicFriction = 0.3f;
    
    PhysicsMaterial() = default;
    PhysicsMaterial(float dens, float rest, float sFrict, float dFrict)
        : density(dens), restitution(rest), staticFriction(sFrict), dynamicFriction(dFrict) {}
    
    static PhysicsMaterial Default() { return PhysicsMaterial(1.0f, 0.5f, 0.5f, 0.3f); }
    static PhysicsMaterial Bouncy()   { return PhysicsMaterial(1.0f, 0.9f, 0.1f, 0.1f); }
    static PhysicsMaterial Rough()    { return PhysicsMaterial(1.0f, 0.2f, 0.8f, 0.6f); }
};

} // namespace engine
