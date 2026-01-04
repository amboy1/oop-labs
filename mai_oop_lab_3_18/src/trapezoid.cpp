#include "trapezoid.h"

Trapezoid::Trapezoid(double a, double b, double height, std::string_view description)
    : Figure(description), m_a(a), m_b(b), m_height(height) {}

Trapezoid::Trapezoid(const Trapezoid& other)
    : Figure(other), m_a(other.m_a), m_b(other.m_b), m_height(other.m_height) {}

Trapezoid::Trapezoid(Trapezoid&& other) noexcept
    : Figure(std::move(other)), m_a(other.m_a), m_b(other.m_b), m_height(other.m_height) 
{
    other.m_a = 0; other.m_b = 0; other.m_height = 0;
}

std::pair<double, double> Trapezoid::computeCenter() const {
    // Центр по оси X — среднее между основаниями, по Y — половина высоты
    return { (m_a + m_b) / 4.0, m_height / 2.0 };
}

void Trapezoid::printVertices(std::ostream& os) const {
    double dx = (m_a - m_b) / 2.0;
    os << "(0,0), (" << m_a << ",0), (" << m_b + dx << "," << m_height
       << "), (" << dx << "," << m_height << ")";
}

Trapezoid::operator double() const {
    return 0.5 * (m_a + m_b) * m_height;
}

bool Trapezoid::equals(const Figure& other) const {
    const Trapezoid* t = dynamic_cast<const Trapezoid*>(&other);
    return t && m_a == t->m_a && m_b == t->m_b && m_height == t->m_height;
}

Figure* Trapezoid::clone() const {
    return new Trapezoid(*this);
}

Trapezoid& Trapezoid::operator=(const Trapezoid& other) {
    if (this != &other) {
        m_a = other.m_a;
        m_b = other.m_b;
        m_height = other.m_height;
    }
    return *this;
}

Trapezoid& Trapezoid::operator=(Trapezoid&& other) noexcept {
    if (this != &other) {
        m_a = other.m_a;
        m_b = other.m_b;
        m_height = other.m_height;
        other.m_a = 0; other.m_b = 0; other.m_height = 0;
    }
    return *this;
}

std::istream& operator>>(std::istream& is, Trapezoid& t) {
    return is >> t.m_a >> t.m_b >> t.m_height;
}

std::ostream& operator<<(std::ostream& os, const Trapezoid& t) {
    t.printVertices(os);
    return os;
}
