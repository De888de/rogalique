#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rogalique Game");
    window.setFramerateLimit(60);
    
    // Load logo from absolute path
    sf::Texture logoTexture;
    sf::Sprite logoSprite;
    
    // Try multiple paths
    std::vector<std::string> paths = {
        "D:/xyz/roqalique/x64/Debug/Resources/xyz-logo.png",
        "D:/xyz/roqalique/x64/Debug/xyz-logo.png",
        "Resources/xyz-logo.png",
        "xyz-logo.png"
    };
    
    bool loaded = false;
    for (const auto& path : paths) {
        if (logoTexture.loadFromFile(path)) {
            std::cout << "Logo loaded from: " << path << std::endl;
            loaded = true;
            break;
        }
    }
    
    if (!loaded) {
        std::cout << "Error: Could not load logo from any path!" << std::endl;
    } else {
        logoSprite.setTexture(logoTexture);
        // Center the logo
        sf::FloatRect bounds = logoSprite.getLocalBounds();
        logoSprite.setOrigin(bounds.width / 2, bounds.height / 2);
        logoSprite.setPosition(400, 200);
        
        // Scale if needed
        float scaleX = 400.0f / bounds.width;
        float scaleY = 200.0f / bounds.height;
        float scale = std::min(scaleX, scaleY);
        logoSprite.setScale(scale, scale);
    }
    
    // Menu items
    std::vector<std::string> menuItems = {"START GAME", "LOAD GAME", "OPTIONS", "EXIT"};
    std::vector<sf::Text> menuTexts;
    int selectedItem = 0;
    
    for (size_t i = 0; i < menuItems.size(); ++i) {
        sf::Text text;
        text.setString(menuItems[i]);
        text.setCharacterSize(36);
        text.setFillColor(sf::Color::White);
        text.setPosition(350, 400 + i * 60);
        menuTexts.push_back(text);
    }
    
    if (!menuTexts.empty()) {
        menuTexts[selectedItem].setFillColor(sf::Color::Yellow);
    }
    
    // Game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    menuTexts[selectedItem].setFillColor(sf::Color::White);
                    selectedItem = (selectedItem - 1 + menuItems.size()) % menuItems.size();
                    menuTexts[selectedItem].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    menuTexts[selectedItem].setFillColor(sf::Color::White);
                    selectedItem = (selectedItem + 1) % menuItems.size();
                    menuTexts[selectedItem].setFillColor(sf::Color::Yellow);
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (menuItems[selectedItem] == "EXIT") {
                        window.close();
                    }
                    else if (menuItems[selectedItem] == "START GAME") {
                        std::cout << "Starting game..." << std::endl;
                    }
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
        
        // Render
        window.clear(sf::Color(20, 20, 40));
        
        // Draw logo
        if (loaded) {
            window.draw(logoSprite);
        } else {
            sf::Text logoText;
            logoText.setString("ROGALIQUE");
            logoText.setCharacterSize(48);
            logoText.setFillColor(sf::Color::Red);
            logoText.setPosition(300, 150);
            window.draw(logoText);
        }
        
        // Draw menu
        for (const auto& text : menuTexts) {
            window.draw(text);
        }
        
        window.display();
    }
    
    return 0;
}
