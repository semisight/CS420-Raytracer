#ifndef VMATH_H
#define VMATH_H

#include <iostream>

/* Generalized 3-vector struct. */
struct vec3 {
    float x;
    float y;
    float z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float i, float j, float k) : x(i), y(j), z(k) {}

    /* Dot and cross products. Non-operator overload functions. */
    static float dot(const vec3 &a, const vec3 &b);
    static vec3 cross(const vec3 &a, const vec3 &b);
    static float length(const vec3 &a);
    static vec3 normalize(const vec3 &a);
    static vec3 clamp(const vec3 &a);
};

/* this can also represent a vector. */
typedef vec3 point;
typedef vec3 vector;

/* Ray type. */
struct ray {
    point origin;
    vector direction; /* Always normalized. */

    ray(point o, vector d) : origin(o) { direction = vector::normalize(d); }

    point at(float t);
};

/* Some handy arithmetic operators for points/vectors. */
bool operator==(const point &a, const point &b);
bool operator!=(const point &a, const point &b);
point operator+(const point &a, const point &b);
point operator-(const point &a, const point &b);
point operator*(const point &a, const point &b);
point operator*(float a, const point &b);

point operator-(const point &a);

/* Stream operator for output in iostreams. */
std::ostream &operator<<(std::ostream &os, const point &p);

#endif
