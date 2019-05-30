#include "game.hpp"

Point::Point( float X, float Y ) {
    x = X;
    y = Y;
}

Point::Point() {
    y = x = 0.0;
}

Point Point::operator=(const Point& p) {
    x = p.x;
    y = p.y;
    return *this;
}

void Point::Rotate( float radians ) {
    float sine = sin(radians);
    float cosine = cos(radians);

    float rx = x * cosine - y * sine;
    float ry = x * sine + y * cosine;

    x = rx;
    y = ry;
}
