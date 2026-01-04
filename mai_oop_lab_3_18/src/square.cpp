#include "square.h"
#include <iomanip>

Square::Square(double side, std::string_view description)
    : Figure(description), m_side(side) {}

Square::Square(const Square& other)
    : Figure(other), m_side(other.m_side) {}

Square::Square(Square&& other) noexcept
    : Figure(std::move(other)), m_side(other.m_side) 
{
    other.m_side = 0;
}

std::pair<double, double> Square::computeCenter() const {
    return { m_side / 2.0, m_side / 2.0 };
}

void Square::printVertices(std::ostream& os) const {
    os << "(" << 0 << "," << 0 << "), (" << m_side << "," << 0 << "), ("
       << m_side << "," << m_side << "), (" << 0 << "," << m_side << ")";
}

Square::operator double() const {
    return m_side * m_side;
}

bool Square::equals(const Figure& other) const {
    const Square* s = dynamic_cast<const Square*>(&other);
    return s && m_side == s->m_side;
}

Figure* Square::clone() const {
    return new Square(*this);
}

Square& Square::operator=(const Square& other) {
    if (this != &other) {
        m_side = other.m_side;
    }
    return *this;
}

Square& Square::operator=(Square&& other) noexcept {
    if (this != &other) {
        m_side = other.m_side;
        other.m_side = 0;
    }
    return *this;
}

std::istream& operator>>(std::istream& is, Square& s) {
    return is >> s.m_side;
}

std::ostream& operator<<(std::ostream& os, const Square& s) {
    s.printVertices(os);
    return os;
}
