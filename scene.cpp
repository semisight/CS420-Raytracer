#include "scene.h"

#include <cassert>
#include <iostream>

using namespace std;

void readDouble(ifstream &, string, double &);
void readVec3(ifstream &, string, vec3 &);

void readDouble(ifstream &in, string check, double &d) {
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
