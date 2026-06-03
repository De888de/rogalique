#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

namespace rogalique
{
    class SoundManager
    {
    public:
        static SoundManager& GetInstance();

        void LoadSound(const std::string& name, const std::string& filename);
        void PlaySound(const std::string& name);
        
        void LoadMusic(const std::string& name, const std::string& filename);
        void PlayMusic(const std::string& name);
        void PlayMusicFile(const std::string& filename);
        void StopMusic();
        
        void SetMusicVolume(float volume);
        void SetSoundVolume(float volume);

    private:
        SoundManager() = default;
        ~SoundManager() = default;

        std::map<std::string, sf::SoundBuffer> m_soundBuffers;
        std::map<std::string, sf::Sound> m_sounds;
        std::map<std::string, std::unique_ptr<sf::Music>> m_musicMap;
        sf::Music m_currentMusic;
        std::string m_currentMusicName;
    };
}
