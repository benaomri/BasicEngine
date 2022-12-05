
#ifndef GAME_LIGHT_H
#define GAME_LIGHT_H


#include "glm/glm.hpp"

using namespace glm;

enum lightType {
    Directional,
    Spot
};

class Light {
public:
    lightType liType;
    vec3 direction;
    vec3 position;
    float cosAngle;
    vec3 rgb_intensity;
    float shiness;
    virtual void setIntensity(vec4 intensity);
};


#endif //GAME_LIGHT_H
