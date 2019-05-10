#include "game.hpp"

Point::Point( double X, double Y ) {
    x = X;
    y = Y;
}

Point::Point() {
    y = x = 0.0;
}

Point Point::operator=(const Point& p) {
    this->x = p.x;
    this->y = p.y;
}


void Point::Rotate( double radians ) {
    double sine = sin(radians);
    double cosine = cos(radians);

    double rx = x * cosine - y * sine;
    double ry = x * sine + y * cosine;

    x = rx;
    y = ry;
}

/*
class Point {
    double x;
    double y;
    Point( double X, double Y );
    void Rotate( double radians );
};
*/