#ifndef SCENE_H
#define SCENE_H

#include <cfloat>
#include <deque>
#include <fstream>
#include <string>

#include "objects.h"
#include "vmath.h"

typedef unsigned char u8;
typedef std::function<void(int, int, u8, u8, u8)> pixelFn;

enum class sampleType { X1, SGSSAA, RGSSAA };

struct screen {
    int width, height;
    float aspect;
    float fov;
    pixelFn putPixel;
    sampleType sampling;
    size_t num_reflections;

    screen(int w, int h, float f, pixelFn p, sampleType s, size_t rs);
};

struct fragment {
    point position;
    color color_diffuse;
    color color_specular;
    vector normal;
    vector camera;
    float shininess;

    fragment() {}
    fragment(point p, color d, color s, vector n, vector c, float sh) :
        position(p), color_diffuse(d), color_specular(s), normal(n), camera(c),
        shininess(sh) {}
};

class scene {
private:
    std::deque<triangle> triangles;
    std::deque<sphere> spheres;
    std::deque<light> lights;
    color ambientLight;

    void readSphere(std::ifstream &in);
    void readTriangle(std::ifstream &in);
    void readLight(std::ifstream &in);

    /* Object intersection code. */
    bool intersectsSphere(sphere sph, ray u, fragment &frag) const;
    bool intersectsTriangle(triangle tri, ray u, fragment &frag) const;
    bool intersectsObject(ray u, fragment &frag, float stop_limit = FLT_MAX) const;

    /* Lighting/shading code. */
    color getLightContribution(const light li, const fragment frag) const;
    color shadeFragment(const ray &u, size_t reflects) const;
    color shadeFragment(const float &x, const float &y, const screen &screen) const;

    /* Sampling. */
    color sample(const int x, const int y, const screen &screen);

    static ray screenToRay(const float &x, const float &y, const screen &screen);

public:
    scene(std::string path);

    void render(screen screen);
};

#endif
