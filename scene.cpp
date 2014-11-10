#include "scene.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include <OpenGL/gl.h>

using namespace std;

screen::screen(int w, int h, float f, pixelFn p) :
    width(w), height(h), putPixel(p) {
    fov = f * (float)M_PI / 180.0f;
    aspect = (float)width/(float)height;
}


void readDouble(ifstream &, string, float &);
void readVec3(ifstream &, string, vec3 &);

void readDouble(ifstream &in, string check, float &d) {
    string noun;

    in >> noun;
    assert(noun.compare(check) == 0);

    in >> d;

    cout << noun << " " << d << endl;
}


void readVec3(ifstream &in, string check, vec3 &vec) {
    string noun;

    in >> noun;
    assert(noun.compare(check) == 0);

    in >> vec.x >> vec.y >> vec.z;

    cout << noun << " " << vec << endl;
}


void scene::readTriangle(ifstream &in) {
    triangle tri;

    cout << "found triangle" << endl;

    // Read vertices.
    for(int i = 0; i < 3; ++i) {
        vertex &v = tri.vertices[i];

        readVec3(in, "pos:", v.position);
        readVec3(in, "nor:", v.normal);
        readVec3(in, "dif:", v.color_diffuse);
        readVec3(in, "spe:", v.color_specular);
        readDouble(in, "shi:", v.shininess);
    }

    triangles.push_back(tri);
}


void scene::readSphere(ifstream &in) {
    sphere sph;

    cout << "found sphere" << endl;

    readVec3(in, "pos:", sph.position);
    readDouble(in, "rad:", sph.radius);
    readVec3(in, "dif:", sph.color_diffuse);
    readVec3(in, "spe:", sph.color_specular);
    readDouble(in, "shi:", sph.shininess);

    spheres.push_back(sph);
}


void scene::readLight(ifstream &in) {
    light li;

    cout << "found light" << endl;

    readVec3(in, "pos:", li.position);
    readVec3(in, "col:", li.color);

    lights.push_back(li);
}


scene::scene(string path) {
    ifstream infile(path);
    int num_objects;

    infile >> num_objects;

    cout << "number of objects: " << num_objects << endl;

    readVec3(infile, "amb:", ambientLight);

    for(int i = 0; i < num_objects; ++i) {
        string noun;

        // What type of row is this?
        infile >> noun;

        if(noun.compare("triangle") == 0) {
            readTriangle(infile);
        } else if(noun.compare("sphere") == 0) {
            readSphere(infile);
        } else if(noun.compare("light") == 0) {
            readLight(infile);
        } else {
            cout << "unknown type in scene description:" << endl << noun << endl;
            exit(0);
        }
    }
}


vector scene::screenToRay(int x, int y, screen screen) {
    vector rv;

    // Compute x and y. z is always -1.
    float tfov = tan(screen.fov/2);

    rv.x = (2*x / (float)screen.width - 1) * screen.aspect * tfov;
    rv.y = (2*y / (float)screen.height - 1) * tfov;
    rv.z = -1;

    return vector::normalize(rv);
}


// Takes ray u starting at u0 and intersects it with sphere sph.
bool scene::intersectsSphere(sphere sph, vector u, point u0, point &inter) {
    // Sphere intersection from geometric queries slides.
    vector dist = u0 - sph.position;
    float b = 2 * (vector::dot(u, dist));
    float c = vector::dot(dist, dist) - sph.radius * sph.radius;

    float discriminant = b*b - 4*c;

    if(discriminant < 0)
        return false;

    float t0 = (-b + sqrt(discriminant)) / 2;
    float t1 = (-b - sqrt(discriminant)) / 2;
    float t = std::min(t0, t1);

    inter = t*u + u0;
    return true;
}


void scene::render(screen screen) {
    // Setup GL.
    glPointSize(2.0);
    glBegin(GL_POINTS);

    for(int y = 0; y < screen.height; ++y) {
        for(int x = 0; x < screen.width; ++x) {
            int r = 0, g = 0, b = 0;
            vector u = screenToRay(x, y, screen);

            for(sphere sph : spheres) {
                point inter;

                if(intersectsSphere(sph, u, point(), inter))
                    r = g = b = 255;
            }

            screen.putPixel(x, y, (unsigned char)r, (unsigned char)g, (unsigned char)b);
        }

        // Hack to render on every nth row.
        if(y % 100 == 0) {
            glEnd();
            glFlush();
            glBegin(GL_POINTS);
        }
    }

    glEnd();
    glFlush();
}
