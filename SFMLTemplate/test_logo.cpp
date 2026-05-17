#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Logo Test");
    sf::Texture tex;
    
    if (tex.loadFromFile("Resources/xyz-logo.png")) {
        std::cout << "Logo loaded!" << std::endl;
        sf::Sprite sprite(tex);
        
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }
            window.clear();
            window.draw(sprite);
            window.display();
        }
    } else {
        std::cout << "Failed to load logo!" << std::endl;
        std::cout << "Current path: " << std::filesystem::current_path() << std::endl;
    }
    return 0;
}
