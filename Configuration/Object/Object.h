#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/glm.hpp"

using namespace glm;
enum objectType {
    Regular,
    Transparent,
    Reflective,
    Space
};
class Object {
public:
    objectType objType;
    vec4 details;
    vec3 rgb_color;
    float shiness;
    int objIndex;

    virtual float FindIntersection(vec3 ray, vec3 somePointOnRay) = 0;
    virtual void setColor(vec4 color);
    virtual vec3 getColor(vec3 hitPoint) = 0;
    virtual float getAngle(vec3 ray, vec3 hitPoint);
    virtual vec3 getNormal(vec3 hitPoint) = 0;
};
vec3 normalizedVector(vec3 vec);

#endif
