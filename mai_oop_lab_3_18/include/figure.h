#ifndef FIGURE_H
#define FIGURE_H

#include <string>
#include <string_view>

class Figure
{
protected:
    // PROTECTED конструкторы - предотвращают прямое создание объектов Shape
    Figure() = default;
    Figure(std::string_view description);

public:
    // ВИРТУАЛЬНЫЙ деструктор - ОБЯЗАТЕЛЕН для полиморфных классов!
    // ВАЖНО: если деструктор не public, нельзя удалять объекты через указатели на базовый класс
    virtual ~Figure() = default;
    
    // ЧИСТЫЕ ВИРТУАЛЬНЫЕ ФУНКЦИИ - делают класс абстрактным
    // = 0 означает, что функция ДОЛЖНА быть переопределена в производных классах
    virtual std::pair<double, double> computeCenter() const = 0;

    virtual void printVertices(std::ostream& os) const = 0;
    virtual operator double() const = 0;

    virtual bool equals(const Figure& other) const = 0;
    virtual Figure* clone() const = 0;

private:
    // Приватное поле для описания фигуры
    std::string m_description;
};

#endif // FIGURE_H