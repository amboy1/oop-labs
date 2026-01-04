#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "figure.h"
#include <iostream>
#include <utility>

class Rectangle : public Figure
{
public:
    Rectangle() = default;
    Rectangle(double width, double height, std::string_view description);

    Rectangle(const Rectangle& other);
    Rectangle(Rectangle&& other) noexcept;
    ~Rectangle() override = default;

    std::pair<double, double> computeCenter() const override;
    void printVertices(std::ostream& os) const override;
    operator double() const override;
    bool equals(const Figure& other) const override;
    Figure* clone() const override;

    Rectangle& operator=(const Rectangle& other);
    Rectangle& operator=(Rectangle&& other) noexcept;

    friend std::istream& operator>>(std::istream& is, Rectangle& rect);
    friend std::ostream& operator<<(std::ostream& os, const Rectangle& rect);

private:
    double m_width{0.0};
    double m_height{0.0};
};

#endif // RECTANGLE_H
