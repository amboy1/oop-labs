#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "figure.h"
#include <iostream>
#include <utility>

class Trapezoid : public Figure
{
public:
    Trapezoid() = default;
    Trapezoid(double a, double b, double height, std::string_view description);

    Trapezoid(const Trapezoid& other);
    Trapezoid(Trapezoid&& other) noexcept;
    ~Trapezoid() override = default;

    std::pair<double, double> computeCenter() const override;
    void printVertices(std::ostream& os) const override;
    operator double() const override;
    bool equals(const Figure& other) const override;
    Figure* clone() const override;

    Trapezoid& operator=(const Trapezoid& other);
    Trapezoid& operator=(Trapezoid&& other) noexcept;

    friend std::istream& operator>>(std::istream& is, Trapezoid& t);
    friend std::ostream& operator<<(std::ostream& os, const Trapezoid& t);

private:
    double m_a{0.0};      // нижнее основание
    double m_b{0.0};      // верхнее основание
    double m_height{0.0}; // высота
};

#endif // TRAPEZOID_H
