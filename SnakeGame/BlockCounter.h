#pragma once
#include <functional>
#include <iostream>

namespace ArkanoidGame
{
    class BlockCounter
    {
    private:
        int totalBlocks = 0;
        int destroyedBlocks = 0;
        std::function<void()> onAllBlocksDestroyed;
        bool allDestroyedTriggered = false;  

    public:
        BlockCounter(std::function<void()> callback)
            : onAllBlocksDestroyed(callback)
        {
        }

        void SetTotalBlocks(int count)
        {
            totalBlocks = count;
            destroyedBlocks = 0;
            allDestroyedTriggered = false;
            std::cout << "[BlockCounter] Total blocks: " << totalBlocks << std::endl;
        }

        void OnBlockDestroyed()
        {
            if (allDestroyedTriggered) return;  

            destroyedBlocks++;
            

            if (destroyedBlocks >= totalBlocks && totalBlocks > 0)
            {
                std::cout << "[BlockCounter] ALL BLOCKS DESTROYED!" << std::endl;
                allDestroyedTriggered = true;
                if (onAllBlocksDestroyed)
                {
                    onAllBlocksDestroyed();
                }
            }
        }

        void Reset()
        {
            destroyedBlocks = 0;
            allDestroyedTriggered = false;
        }
    };
}