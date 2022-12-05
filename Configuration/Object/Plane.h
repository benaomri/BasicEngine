

#ifndef GAME_PLANE_H
#define GAME_PLANE_H

#include "Object.h"

class Plane : public Object{

public:
    Plane(vec4 details, objectType objType);
    vec3 normal();
    float d();
    float FindIntersection(vec3 ray, vec3 somePointOnRay);
    vec3 getColor(vec3 hitPoint);
    float getAngle(vec3 ray, vec3 hitPoint);
    vec3 getNormal(vec3 hitPoint);
};

#endif //GAME_PLANE_H
