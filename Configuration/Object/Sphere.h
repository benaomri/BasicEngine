#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H

#include "Object.h"

class Sphere : public Object {

public:
    Sphere(vec4 details, objectTypes type);
    vec3 center();
    float radius();
    float FindIntersection(Ray ray);
    vec3 getObjectColor(vec3 hitPoint);
    vec3 getNormal(vec3 hitPoint);
};


#endif //GAME_SPHERE_H
