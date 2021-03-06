#include "scene.h"

#include <cassert>
#include <cmath>
#include <iostream>

#include <OpenGL/gl.h>

using namespace std;

screen::screen(int w, int h, float f, pixelFn p, sampleType s, size_t rs) :
    width(w), height(h), putPixel(p), sampling(s), num_reflections(rs) {
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

    /* Read vertices. */
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

        /* What type of row is this? */
        infile >> noun;

        if(noun.compare("triangle") == 0)
            readTriangle(infile);
        else if(noun.compare("sphere") == 0)
            readSphere(infile);
        else if(noun.compare("light") == 0)
            readLight(infile);
        else {
            cout << "unknown type in scene description:" << endl << noun << endl;
            exit(0);
        }
    }
}


ray scene::screenToRay(const float &x, const float &y, const screen &screen) {
    vector d;

    /* Compute x and y. z is always -1. */
    float tfov = tan(screen.fov/2);
    float i = x + 0.5f;
    float j = y + 0.5f;

    /* TODO: add offset for middle of pixel. */
    d.x = (2*i / screen.width  - 1) * screen.aspect * tfov;
    d.y = (2*j / screen.height - 1) * tfov;
    d.z = -1;

    return ray(point(), d);
}


/* Takes ray u and tests for intersection with sphere sph. */
bool scene::intersectsSphere(sphere sph, ray u, fragment &frag) const {
    /* Sphere intersection from geometric queries slides. */
    vector dist = u.origin - sph.position;

    /* First check if ray is cast from inside sphere. */
    if(vector::length(dist) < sph.radius)
        return false;

    float b = 2 * (vector::dot(u.direction, dist));
    float c = vector::dot(dist, dist) - sph.radius * sph.radius;
    float discriminant = b*b - 4*c;

    if(discriminant < 0)
        return false;

    float t0 = (-b + sqrt(discriminant)) / 2;
    float t1 = (-b - sqrt(discriminant)) / 2;
    float t = std::min(t0, t1);

    /* Don't bother if the sphere is behind us or if self-intersection. */
    if(t <= .001)
        return false;

    point p = u.at(t);
    frag = fragment(p, sph.color_diffuse, sph.color_specular,
        vector::normalize(p - sph.position), u.direction, sph.shininess);
    return true;
}


template<typename T>
constexpr bool between(T a, T b, T c);

template<typename T>
constexpr bool between(T a, T b, T c) {
    return (a <= b) && (b <= c);
}


/* Takes a ray u and tests for intersection with triangle tri. */
bool scene::intersectsTriangle(triangle tri, ray u, fragment &frag) const {
    /* First, find the triangle plane. */
    point A = tri.vertices[0].position,
          B = tri.vertices[1].position,
          C = tri.vertices[2].position;
    vector tri_cross = vector::cross(B - A, C - A);
    vector normal = vector::normalize(tri_cross);

    /* Calculate d from the implicit plane equation ax+by+cz+d=0. */
    float dPlane = vector::dot(normal, -A);

    /* Get the normal * direction and check if zero (parallel). */
    float NdotU = vector::dot(normal, u.direction);

    if(abs(NdotU) <= FLT_EPSILON)
        return false;

    /* Calculate our t value (ray direction multiplier) and intersection
       point. */
    float t = -(vector::dot(normal, u.origin) + dPlane) / NdotU;
    point P = u.at(t);

    /* Don't bother if the triangle is behind us. */
    if(t <= .001)
        return false;

    /* Find the area of the triangle. */
    float areaTotal2 = vector::length(tri_cross);
    float alpha = vector::length(vector::cross(B - P, C - P)) / areaTotal2;
    float beta  = vector::length(vector::cross(C - P, A - P)) / areaTotal2;
    float gamma = vector::length(vector::cross(A - P, B - P)) / areaTotal2;

    /* alpha, beta, and gamma must be between 0 and 1 and sum to 1. */
    if(!between(0.f, alpha, 1.f) ||
       !between(0.f, beta, 1.f)  ||
       !between(0.f, gamma, 1.f) ||
       fabs(1.f - alpha - beta - gamma) > .0001)
        return false;

    color diffuse = alpha * tri.vertices[0].color_diffuse +
                    beta  * tri.vertices[1].color_diffuse +
                    gamma * tri.vertices[2].color_diffuse;
    color specular = alpha * tri.vertices[0].color_specular +
                     beta  * tri.vertices[1].color_specular +
                     gamma * tri.vertices[2].color_specular;
    float shininess = alpha * tri.vertices[0].shininess +
                      beta  * tri.vertices[1].shininess +
                      gamma * tri.vertices[2].shininess;
    vector pnormal = vector::normalize(alpha * tri.vertices[0].normal +
                                       beta  * tri.vertices[1].normal +
                                       gamma * tri.vertices[2].normal);

    frag = fragment(P, diffuse, specular, pnormal, u.direction, shininess);
    return true;
}


/* Finds if ray intersects any object in scene. Optionally stops when distance
   under stop_limit is found, and returns that object's data. */
bool scene::intersectsObject(ray u, fragment &frag, float stop_limit) const {
    float dist = FLT_MAX, length = FLT_MAX;
    bool intersects = false;
    fragment tmp;

    for(sphere sph : spheres) {
        if(intersectsSphere(sph, u, tmp)) {
            length = vector::length(tmp.position - u.origin);

            if(length >= stop_limit)
                continue;

            intersects = true;

            if(length < dist) {
                dist = length;
                frag = tmp;
            }
        }
    }

    for(triangle tri : triangles) {
        if(intersectsTriangle(tri, u, tmp)) {
            length = vector::length(tmp.position - u.origin);

            if(length >= stop_limit)
                continue;

            intersects = true;

            if(length < dist) {
                dist = length;
                frag = tmp;
            }
        }
    }

    return intersects;
}


/* Get the contribution from a single light. */
color scene::getLightContribution(const light li, const fragment frag) const {
    vector dir = li.position - frag.position;
    ray toLight(frag.position, dir);
    vector L = toLight.direction;
    fragment tmp;

    /* Don't do anything if it hits something. Return 0. */
    if(intersectsObject(toLight, tmp, vector::length(dir)))
        return color();

    /* Calculate color contribution from diffuse. */
    float LdotN = vector::dot(L, frag.normal);

    /* Calculate color contribution from specular. */
    vector R = vector::normalize(2 * LdotN * frag.normal - L);
    float RdotV = vector::dot(R, -frag.camera);

    LdotN = (LdotN < 0) ? 0 : LdotN;
    RdotV = (RdotV < 0) ? 0 : RdotV;
    
    float RdotVtoa = pow(RdotV, frag.shininess);

    return li.color * (LdotN * frag.color_diffuse +
                       RdotVtoa * frag.color_specular);
}


/* Responsible for aggregating the color that a pixel should be. */
color scene::shadeFragment(const ray &u, size_t reflects) const {
    color final;
    color phong = ambientLight;
    fragment frag;

    /* Test for initial object intersection. */
    if(!intersectsObject(u, frag)) {
        return color(1);
    }

    /* Add in contribution from each light. */
    for(light li : lights) {
        phong = phong + getLightContribution(li, frag);
    }

    /* If calculating reflected light, calculate here. */
    if (reflects != 0) {
        vector V = -u.direction, N = frag.normal;
        vector R = vector::normalize(2 * vector::dot(V, N) * N - V);
        ray uprime(frag.position, R);

        /* Calculate reflection. */
        color reflect = shadeFragment(uprime, reflects-1);

        final = frag.color_specular * reflect +
            (color(1) - frag.color_specular) * phong;
    } else {
        final = phong;
    }

    return color::clamp(final);
}


color scene::shadeFragment(const float &x, const float &y, const screen &screen) const {
    ray u = screenToRay(x, y, screen);
    return shadeFragment(u, screen.num_reflections);
}


/* Gets a sample for a specific point on the screen. Multisampling! */
color scene::sample(const int x, const int y, const screen &screen) {
    color rv;

    constexpr float SGOffset = .3536f;
    constexpr float RG1Offset = .25f;
    constexpr float RG2Offset = .4330f;

    switch(screen.sampling) {
    case sampleType::X1:
        rv = shadeFragment(x, y, screen);
        break;
    case sampleType::SGSSAA:
        rv = rv + shadeFragment(x-SGOffset, y-SGOffset, screen);
        rv = rv + shadeFragment(x+SGOffset, y-SGOffset, screen);
        rv = rv + shadeFragment(x-SGOffset, y+SGOffset, screen);
        rv = rv + shadeFragment(x+SGOffset, y+SGOffset, screen);
        rv = 0.25f * rv;
        break;
    case sampleType::RGSSAA:
        rv = rv + shadeFragment(x+RG1Offset, y+RG2Offset, screen);
        rv = rv + shadeFragment(x-RG2Offset, y+RG1Offset, screen);
        rv = rv + shadeFragment(x-RG1Offset, y-RG2Offset, screen);
        rv = rv + shadeFragment(x+RG2Offset, y-RG1Offset, screen);
        rv = 0.25f * rv;
    }

    return 255 * rv;
}


void scene::render(screen screen) {
    /* Setup GL. */
    glPointSize(2.0);
    glBegin(GL_POINTS);

    for(int y = 0; y < screen.height; ++y) {
        for(int x = 0; x < screen.width; ++x) {
            color final = sample(x, y, screen);

            screen.putPixel(x, y, (unsigned char)final.x,
                                  (unsigned char)final.y,
                                  (unsigned char)final.z);
        }

        /* Hack to render on every nth row. */
        if(y % 100 == 0) {
            glEnd();
            glFlush();
            glBegin(GL_POINTS);
        }
    }

    glEnd();
    glFlush();
}
