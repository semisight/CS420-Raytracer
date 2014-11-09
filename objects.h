#ifndef OBJECTS_H
#define OBJECTS_H

#include "vmath.h"

typedef vec3 color;

struct vertex {
    point position;
    color color_diffuse;
    color color_specular;
    vector normal;
    double shininess;
};

struct triangle {
    vertex vertices[3];
};

struct sphere {
    point position;
    color color_diffuse;
    color color_specular;
    double shininess;
    double radius;
};

struct light {
    point position;
    color color;
};

#endif
