#pragma once
#include <memory>

namespace RoqaliqueGame
{
    class Colladiable   
    {
    public:
        virtual ~Colladiable() = default;

        virtual bool GetCollision(std::shared_ptr<Colladiable> collidableObject) const = 0;
        virtual void OnHit() = 0;
    };
}
