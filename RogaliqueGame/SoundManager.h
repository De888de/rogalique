#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>

namespace rogalique
{
    class SoundManager
    {
    public:
        static SoundManager& GetInstance();

        void LoadSound(const std::string& name, const std::string& filename);
        void PlaySound(const std::string& name);
        void PlayMusic(const std::string& filename);
        void StopMusic();
        void SetMusicVolume(float volume);
        void SetSoundVolume(float volume);

    private:
        SoundManager() = default;
        ~SoundManager() = default;

        std::map<std::string, sf::SoundBuffer> m_soundBuffers;
        std::map<std::string, sf::Sound> m_sounds;
        sf::Music m_music;
    };
}
