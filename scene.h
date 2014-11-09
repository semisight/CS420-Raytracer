#ifndef SCENE_H
#define SCENE_H

#include <deque>
#include <fstream>
#include <string>

#include "objects.h"

class scene {
private:
    std::deque<triangle> triangles;
    std::deque<sphere> spheres;
    std::deque<light> lights;
    color ambientLight;

    void readSphere(std::ifstream &in);
    void readTriangle(std::ifstream &in);
    void readLight(std::ifstream &in);
public:
    scene(std::string path);
};

#endif
