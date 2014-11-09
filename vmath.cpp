#include "vmath.h"

#include <cmath>

/* Dot and cross products. Non-operator overload functions. */
double vec3::dot(const vector &a, const vector &b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}


vector vec3::cross(const vector &a, const vector &b) {
    return point(a.y*b.z - a.z*b.y,
                 a.z*b.x - a.x*b.z,
                 a.x*b.y - a.y*b.x);
}


double vec3::length(const point &a) {
    return sqrt(point::dot(a, a));
}


point vec3::normalize(const point &a) {
    return 1/point::length(a) * a;
}


/* Equality operations. */
bool operator==(const point &a, const point &b) {
    const double epsilon2 = 0.001;
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


/* Scalar multiplication. */
point operator*(double a, const point &b) {
    return point(a * b.x, a * b.y, a * b.z);
}


std::ostream &operator<<(std::ostream &os, const point &p) {
    os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return os;
}
