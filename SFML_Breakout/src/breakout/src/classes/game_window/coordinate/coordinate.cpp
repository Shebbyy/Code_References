#include "coordinate.h"

coordinate::coordinate(const float x, const float y)
    : m_x(x)
    , m_y(y)
{}

float coordinate::get_x() const {
    return m_x;
}

float coordinate::get_y() const {
    return m_y;
}

void coordinate::set_y(const float new_y) {
    m_y = new_y;
}

void coordinate::set_x(const float new_x) {
    m_x = new_x;
}