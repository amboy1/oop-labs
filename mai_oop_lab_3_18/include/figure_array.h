#ifndef FIGUREARRAY_H
#define FIGUREARRAY_H

#include "figure.h"
#include <iostream>
#include <stdexcept>

class FigureArray {
public:
    // Конструкторы
    FigureArray();  // по умолчанию
    FigureArray(const FigureArray& other);  // копирующий
    FigureArray(FigureArray&& other) noexcept;  // перемещающий

    // Деструктор
    ~FigureArray() noexcept;

    // Методы
    void add(Figure* fig);          // добавление фигуры
    void remove(size_t index);      // удаление фигуры по индексу
    double totalArea() const;       // суммарная площадь
    void printAll() const;          // вывод всех фигур и их свойств
    bool equals(const FigureArray& other) const;  // сравнение массивов по количеству фигур

    size_t size() const { return arraySize; }

private:
    Figure** dataArray;
    size_t arraySize;
};

#endif // FIGUREARRAY_H
