#pragma once

namespace RoqaliqueGame
{
    class Game
    {
    public:
        Game();
        ~Game();
        void Run();
        
        // Заглушки для методов, которые нужны другим классам
        bool HasSavedProgress() const { return false; }
        void StartGame() {}
        void ContinueGame() {}
    };
}
