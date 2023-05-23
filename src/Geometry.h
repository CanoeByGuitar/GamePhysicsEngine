//
// Created by chenhui on 23-5-23.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GEOMETRY_H
#define GAMEPHYSICSINONEWEEKEND_GEOMETRY_H

#include <Base/Log.h>

struct Line{
    vec3 start;
    vec3 end;
    Line(const vec3& s, const vec3& e): start(s), end(e){}
};

struct Ray{
    vec3 origin;
    vec3 direction;

    Ray(const vec3& o, const vec3& d) : origin(o), direction(glm::normalize(d)){}
};

struct Sphere{
    vec3 position;
    float radius;

    Sphere(const vec3& p, float r):position(p), radius(r){}
};

struct AABB{
    vec3 position;
    vec3 halfSize;
    AABB(const vec3& p, const vec3& s): position(p), halfSize(s){}
};

struct OBB{
    // oriented OBB
    vec3 position;
    vec3 halfSize;
    mat3 orientation;

    OBB(const vec3& p, const vec3& s):position(p), halfSize(s), orientation(mat3()) {}
    OBB(const vec3& p, const vec3& s, const mat3& o):position(p), halfSize(s), orientation(o) {}

};

struct Plane{
    vec3 normal;
    float distance;

    Plane():normal(vec3(0, 1, 0)), distance(0.f){}
    Plane(const vec3& n, float d) : normal(n), distance(d) {}
};

struct Triangle{
    union{
        struct{
            vec3 a;
            vec3 b;
            vec3 c;
        };
        struct{
            vec3 p1;
            vec3 p2;
            vec3 p3;
        };
        vec3 points[3];
    };

    Triangle(const vec3& a, const vec3& b, const vec3& c): a(a), b(b), c(c){}
};




















#endif //GAMEPHYSICSINONEWEEKEND_GEOMETRY_H
