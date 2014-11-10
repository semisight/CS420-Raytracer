#ifndef SCENE_H
#define SCENE_H

#include <deque>
#include <fstream>
#include <string>

#include "objects.h"
#include "vmath.h"

typedef unsigned char u8;
typedef std::function<void(int, int, u8, u8, u8)> pixelFn;

struct screen {
    int width, height;
    float aspect;
    float fov;
    pixelFn putPixel;

    screen(int w, int h, float f, pixelFn p);
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

    bool intersectsSphere(sphere sph, vector u, point u0, point &inter);

    static point screenToRay(int x, int y, screen screen);

public:
    scene(std::string path);

    void render(screen screen);
};

#endif
