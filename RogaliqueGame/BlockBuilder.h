#pragma once
#include <string>

namespace rogalique
{
    class BlockBuilder
    {
    public:
        static void LoadLevel(const std::string& filename, float worldWidth, float worldHeight, float blockSize);
    };
}
