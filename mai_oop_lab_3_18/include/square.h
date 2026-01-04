#ifndef SQUARE_H
#define SQUARE_H

#include "figure.h"
#include <iostream>
#include <utility>

class Square : public Figure
{
public:
    Square() = default;
    Square(double side, std::string_view description);

    Square(const Square& other);
    Square(Square&& other) noexcept;
    ~Square() override = default;

    std::pair<double, double> computeCenter() const override;
    void printVertices(std::ostream& os) const override;
    operator double() const override;
    bool equals(const Figure& other) const override;
    Figure* clone() const override;

    Square& operator=(const Square& other);
    Square& operator=(Square&& other) noexcept;

    friend std::istream& operator>>(std::istream& is, Square& s);
    friend std::ostream& operator<<(std::ostream& os, const Square& s);

private:
    double m_side{0.0};
};

#endif // SQUARE_H
