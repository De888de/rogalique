#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Player.h"

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
        void Draw(sf::RenderWindow& window);
        void Pause();
        void Resume();
        
        bool HasSavedProgress() const;
        
    private:
        std::unique_ptr<Player> m_player;
        bool m_hasSave;
        bool m_isPaused;
    };
    
    extern Game* g_Game;
}
