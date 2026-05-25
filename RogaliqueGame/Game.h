#pragma once

namespace rogalique
{
    class Game
    {
    public:
        Game();
        ~Game();
        
        void StartNewGame();
        void ContinueGame();
        void Update(float deltaTime);
        void Pause();
        void Resume();
        
        bool HasSavedProgress() const;
    };
    
    extern Game* g_Game;
}
