#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "glm/glm.hpp"
#include "../Utils/Ray.h"

using namespace glm;
enum objectType {
    Regular,
    Transparent,
    Reflective,
    Space
};
class Object {
public:
    objectType type;
    vec4 details;
    vec3 rgbColor;
    float shiness;
    int objIndex;

    virtual float FindIntersection(Ray ray) = 0;
    virtual void setColor(vec4 color);
    virtual vec3 getColor(vec3 hitPoint) = 0;
    virtual vec3 getNormal(vec3 hitPoint) = 0;
};
vec3 normalizedVector(vec3 vec);

#endif
