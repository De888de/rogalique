#include "Application.h"
#include "TransformMatrix.h"
#include <iostream>

int main()
{
    // Тест матриц
    rogalique::TransformMatrix translation;
    translation.SetTranslation(12, 5);
    std::cout << "Translation matrix:\n";
    translation.Print();

    rogalique::TransformMatrix rotation;
    rotation.SetRotation(90);
    std::cout << "\nRotation matrix:\n";
    rotation.Print();

    auto combined = rotation * translation;
    std::cout << "\nCombined (rotation * translation):\n";
    combined.Print();

    // Запуск игры
    rogalique::Application app;
    app.Run();

    return 0;
}