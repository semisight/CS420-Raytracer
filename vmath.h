#ifndef VMATH_H
#define VMATH_H

#include <iostream>

/* Generalized 3-vector struct. */
struct vec3 {
    double x;
    double y;
    double z;

    vec3() : x(0), y(0), z(0) {}
    vec3(double i, double j, double k) : x(i), y(j), z(k) {}

    /* Dot and cross products. Non-operator overload functions. */
    static double dot(const vec3 &a, const vec3 &b);
    static vec3 cross(const vec3 &a, const vec3 &b);
    static double length(const vec3 &a);
    static vec3 normalize(const vec3 &a);
};

/* this can also represent a vector. */
typedef vec3 point;
typedef vec3 vector;

/* Some handy arithmetic operators for points/vectors. */
bool operator==(const point &a, const point &b);
bool operator!=(const point &a, const point &b);
point operator+(const point &a, const point &b);
point operator-(const point &a, const point &b);
point operator*(double a, const point &b);

/* Stream operator for output in iostreams. */
std::ostream &operator<<(std::ostream &os, const point &p);

#endif
