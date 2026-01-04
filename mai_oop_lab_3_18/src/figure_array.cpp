#include "figure_array.h"
#include "rectangle.h"
#include "trapezoid.h"
#include "square.h"

#include <iostream>

// === Конструкторы ===

// По умолчанию
FigureArray::FigureArray() : dataArray(nullptr), arraySize(0) {}

// Копирующий (глубокое копирование)
FigureArray::FigureArray(const FigureArray& other) : arraySize(other.arraySize) {
    dataArray = new Figure*[arraySize];
    for (size_t i = 0; i < arraySize; ++i) {
        dataArray[i] = other.dataArray[i]->clone();
    }
}

// Перемещающий
FigureArray::FigureArray(FigureArray&& other) noexcept 
    : dataArray(other.dataArray), arraySize(other.arraySize) {
    other.dataArray = nullptr;
    other.arraySize = 0;
}

// Деструктор
FigureArray::~FigureArray() noexcept {
    if (dataArray) {
        for (size_t i = 0; i < arraySize; ++i)
            delete dataArray[i];
        delete[] dataArray;
    }
    dataArray = nullptr;
    arraySize = 0;
}

// === Методы ===

// Добавление фигуры
void FigureArray::add(Figure* fig) {
    Figure** newArray = new Figure*[arraySize + 1];
    for (size_t i = 0; i < arraySize; ++i)
        newArray[i] = dataArray[i];
    newArray[arraySize] = fig;
    delete[] dataArray;
    dataArray = newArray;
    ++arraySize;
}

// Удаление фигуры по индексу
void FigureArray::remove(size_t index) {
    if (index >= arraySize)
        throw std::out_of_range("Индекс вне диапазона");

    delete dataArray[index];

    for (size_t i = index; i < arraySize - 1; ++i)
        dataArray[i] = dataArray[i + 1];

    --arraySize;
}

// Суммарная площадь
double FigureArray::totalArea() const {
    double sum = 0.0;
    for (size_t i = 0; i < arraySize; ++i)
        sum += static_cast<double>(*dataArray[i]);
    return sum;
}

// Вывод всех фигур
void FigureArray::printAll() const {
    for (size_t i = 0; i < arraySize; ++i) {
        // Определяем тип фигуры
        std::string typeName = "Unknown";
        if (dynamic_cast<Rectangle*>(dataArray[i])) {
            typeName = "Rectangle";
        } else if (dynamic_cast<Trapezoid*>(dataArray[i])) {
            typeName = "Trapezoid";
        } else if (dynamic_cast<Square*>(dataArray[i])) {
            typeName = "Square";
        }
        
        std::cout << i << ": " << typeName << " - " << *dataArray[i] << "\n";
        auto center = dataArray[i]->computeCenter();
        double area = static_cast<double>(*dataArray[i]);
        std::cout << "   Центр: (" << center.first << ", " << center.second << "), Площадь: " << area << "\n";
    }
}

// Сравнение массивов по количеству фигур
bool FigureArray::equals(const FigureArray& other) const {
    return arraySize == other.arraySize;
}
