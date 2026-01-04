#include "rectangle.h"

Rectangle::Rectangle(double width, double height, std::string_view description)
    : Figure(description), m_width(width), m_height(height) {}

Rectangle::Rectangle(const Rectangle& other)
    : Figure(other), m_width(other.m_width), m_height(other.m_height) {}

Rectangle::Rectangle(Rectangle&& other) noexcept
    : Figure(std::move(other)), m_width(other.m_width), m_height(other.m_height)
{
    other.m_width = 0;
    other.m_height = 0;
}

std::pair<double, double> Rectangle::computeCenter() const {
    return { m_width / 2.0, m_height / 2.0 };
}

void Rectangle::printVertices(std::ostream& os) const {
    os << "(0,0), (" << m_width << ",0), (" << m_width << "," << m_height
       << "), (0," << m_height << ")";
}

Rectangle::operator double() const {
    return m_width * m_height;
}

bool Rectangle::equals(const Figure& other) const {
    const Rectangle* r = dynamic_cast<const Rectangle*>(&other);
    return r && m_width == r->m_width && m_height == r->m_height;
}

Figure* Rectangle::clone() const {
    return new Rectangle(*this);
}

Rectangle& Rectangle::operator=(const Rectangle& other) {
    if (this != &other) {
        m_width = other.m_width;
        m_height = other.m_height;
    }
    return *this;
}

Rectangle& Rectangle::operator=(Rectangle&& other) noexcept {
    if (this != &other) {
        m_width = other.m_width;
        m_height = other.m_height;
        other.m_width = 0;
        other.m_height = 0;
    }
    return *this;
}

std::istream& operator>>(std::istream& is, Rectangle& rect) {
    return is >> rect.m_width >> rect.m_height;
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rect) {
    rect.printVertices(os);
    return os;
}
