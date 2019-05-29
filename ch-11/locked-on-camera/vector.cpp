#include "game.hpp"

Vector2D::Vector2D( float X, float Y ) {
    x = X;
    y = Y;
}

Vector2D::Vector2D() {
    y = x = 0.0;
}

Vector2D Vector2D::operator=(const Vector2D& p) {
    x = p.x;
    y = p.y;
    return *this;
}

void Vector2D::operator+=(const Vector2D& p) {
    x += p.x;
    y += p.y;
}

void Vector2D::operator-=(const Vector2D& p) {
    x -= p.x;
    y -= p.y;
}

void Vector2D::operator*=(const float& scalar) {
    x *= scalar;
    y *= scalar;
}

void Vector2D::operator/=(const float& scalar) {
    x /= scalar;
    y /= scalar;
}

Vector2D Vector2D::operator*(const float& scalar) {
    Vector2D vec = *this;
    vec *= scalar;
    return vec;
}

void Vector2D::Rotate( float radians ) {
    float sine = sin(radians);
    float cosine = cos(radians);

    float rx = x * cosine - y * sine;
    float ry = x * sine + y * cosine;

    x = rx;
    y = ry;
}

void Vector2D::Normalize() {
    float mag = Magnitude();

    x /= mag;
    y /= mag;
}

Vector2D Vector2D::Project(Vector2D &onto) {
    Vector2D proj = *this;

    float proj_dot_onto = proj.Dot(onto);

    proj *= proj_dot_onto;
    return proj;
}

float Vector2D::Dot(Vector2D &vec) {
    Vector2D this_norm;
    this_norm = *this;
    this_norm.Normalize();

    Vector2D vec_norm;
    vec_norm = vec;
    vec_norm.Normalize();
    
    return this_norm.x * vec_norm.x + this_norm.y * vec_norm.y;
}
float Vector2D::FindAngle() {
    if( x == 0.0 && y == 0.0 ) {
        return 0.0;
    }

    Vector2D this_norm;
    this_norm = *this;
    this_norm.Normalize();

    return atan2( this_norm.y, this_norm.x ) + PI / 2;
}

float Vector2D::MagSQ() {
    return x * x + y * y;
}

float Vector2D::Magnitude() {
    return sqrt( MagSQ() );
}

Vector2D Vector2D::operator-(const Vector2D &vec) {
    Vector2D return_vec = *this;
    return_vec -= vec;
    return return_vec;
}

Vector2D Vector2D::operator+(const Vector2D &vec) {
    Vector2D return_vec = *this;
    return_vec += vec;
    return return_vec;
}
