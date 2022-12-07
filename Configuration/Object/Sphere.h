#ifndef GAME_SPHERE_H
#define GAME_SPHERE_H

#include "Object.h"

class Sphere : public Object {

public:
    Sphere(vec4 details, objectType objType);
    vec3 center();
    float radius();
    float FindIntersection(vec3 ray, vec3 somePointOnRay);
    vec3 getColor(vec3 hitPoint);
    float getAngle(vec3 ray, vec3 hitPoint);
    vec3 getNormal(vec3 hitPoint);
};


#endif //GAME_SPHERE_H
