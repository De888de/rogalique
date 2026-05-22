// ©2023, XYZ School. All rights reserved.
// Authored by Aleksandr Rybalka (polterageist@gmail.com)

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Engine.h"

const std::string RESOURCES_PATH = "Resources/";

int main()
{
    Engine engine;

    // Исправлено: добавил 3 аргумента
    if (!engine.Initialize("Rogalique Game", 1024, 768))
    {
        return EXIT_FAILURE;
    }

    engine.Run();

    return 0;
}