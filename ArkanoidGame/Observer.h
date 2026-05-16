#pragma once
#include <functional>
#include <vector>
#include <iostream>

namespace ArkanoidGame
{
    enum class GameEvent
    {
        BlockDestroyed,
        AllBlocksDestroyed
    };

    class GameEventDispatcher
    {
    private:
        std::vector<std::function<void(GameEvent, int)>> listeners;

    public:
        void AddListener(std::function<void(GameEvent, int)> listener)
        {
            listeners.push_back(listener);
        }

        void Dispatch(GameEvent event, int value = 0)
        {
            // Создаём копию списка, чтобы избежать проблем при удалении во время итерации
            auto listenersCopy = listeners;
            for (auto& listener : listenersCopy)
            {
                listener(event, value);
            }
        }

        void Clear()
        {
            listeners.clear();
        }
    };
}