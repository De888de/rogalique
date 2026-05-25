#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace rogalique
{
    class ResourceSystem
    {
    public:
        static ResourceSystem* Instance();
        
        // Текстуры
        void LoadTexture(const std::string& name, const std::string& sourcePath, bool isSmooth = true);
        const sf::Texture* GetTextureShared(const std::string& name) const;
        sf::Texture* GetTextureCopy(const std::string& name) const;
        void DeleteSharedTexture(const std::string& name);
        
        // Текстурные атласы (карты)
        void LoadTextureMap(const std::string& name, const std::string& sourcePath, 
                            sf::Vector2u elementPixelSize, int totalElements, bool isSmooth = true);
        const sf::Texture* GetTextureMapElementShared(const std::string& name, int elementIndex) const;
        sf::Texture* GetTextureMapElementCopy(const std::string& name, int elementIndex) const;
        int GetTextureMapElementsCount(const std::string& name) const;
        void DeleteSharedTextureMap(const std::string& name);
        
        // Шрифты
        void LoadFont(const std::string& name, const std::string& sourcePath);
        const sf::Font* GetFont(const std::string& name) const;
        
        // Звуки
        void LoadSoundBuffer(const std::string& name, const std::string& sourcePath);
        const sf::SoundBuffer* GetSoundBuffer(const std::string& name) const;
        
        void Clear();
        
    private:
        ResourceSystem() = default;
        ~ResourceSystem();
        
        ResourceSystem(ResourceSystem const&) = delete;
        ResourceSystem& operator=(ResourceSystem const&) = delete;
        
        void DeleteAllTextures();
        void DeleteAllTextureMaps();
        
        std::map<std::string, sf::Texture*> m_textures;
        std::map<std::string, std::vector<sf::Texture*>> m_textureMaps;
        std::map<std::string, sf::Font*> m_fonts;
        std::map<std::string, sf::SoundBuffer*> m_soundBuffers;
    };
}
