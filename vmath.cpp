#include "vmath.h"

#include <cmath>

/* Dot and cross products. Non-operator overload functions. */
float vec3::dot(const vector &a, const vector &b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}


vector vec3::cross(const vector &a, const vector &b) {
    return point(a.y*b.z - a.z*b.y,
                 a.z*b.x - a.x*b.z,
                 a.x*b.y - a.y*b.x);
}


float vec3::length(const point &a) {
    return (float)sqrt(point::dot(a, a));
}


point vec3::normalize(const point &a) {
    return 1/point::length(a) * a;
}


vec3 vec3::clamp(const vec3 &a) {
    vec3 rv = a;

    rv.x = (rv.x > 1) ? 1 : ((rv.x < 0) ? 0 : rv.x);
    rv.y = (rv.y > 1) ? 1 : ((rv.y < 0) ? 0 : rv.y);
    rv.z = (rv.z > 1) ? 1 : ((rv.z < 0) ? 0 : rv.z);
    return rv;
}


/* Evaluate the ray to find the point at multiplier t. */
point ray::at(float t) {
    return t*direction + origin;
}


/* Equality operations. */
bool operator==(const point &a, const point &b) {
    const float epsilon2 = 0.001f;
    vector diff = b - a;

    return (point::dot(diff, diff) < epsilon2);
}


bool operator!=(const point &a, const point &b) {
    return !(a == b);
}


/* Addition / subtraction. */
point operator+(const point &a, const point &b) {
    return point(a.x + b.x, a.y + b.y, a.z + b.z);
}


point operator-(const point &a, const point &b) {
    return point(a.x - b.x, a.y - b.y, a.z - b.z);
}


/* Element-wise multiplication. */
point operator*(const point &a, const point &b) {
    return point(a.x * b.x, a.y * b.y, a.z * b.z);
}


/* Negation. */
point operator-(const point &a) {
    return point() - a;
}


/* Scalar multiplication. */
point operator*(float a, const point &b) {
    return point(a * b.x, a * b.y, a * b.z);
}


std::ostream &operator<<(std::ostream &os, const point &p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}
