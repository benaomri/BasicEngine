#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H

#include "glm/glm.hpp"

using namespace glm;

enum lightTypes {
    Directional,
    Spot
};

class Light {
public:
    lightTypes lightType;
    vec3 lightDirection;
    vec3 lightPosition;
    float lightCosAngle;
    vec3 lightRgbIntensity;
    float lightShiness;
    virtual void setIntensity(vec4 lightIntensity);
};

#endif //GAME_LIGHT_H
