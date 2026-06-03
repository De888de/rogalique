#pragma once
#include <array>
#include <iostream>

namespace rogalique
{
    class TransformMatrix
    {
    public:
        TransformMatrix(); // Единичная матрица

        // Создание матрицы из значений
        TransformMatrix(const std::array<std::array<float, 3>, 3>& values);

        TransformMatrix& operator=(const TransformMatrix& other);

        // Применение к точке (x, y)
        void Transform(float& x, float& y) const;

        // Сеттеры для преобразований
        void SetTranslation(float tx, float ty);  // Передвижение
        void SetRotation(float angleDeg);        // Поворот
        void SetScale(float sx, float sy);       // Масштаб

        void Print() const; // Вывод в консоль (для отладки)

    private:
        std::array<std::array<float, 3>, 3> m;
    };
}
