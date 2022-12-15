
#ifndef GAME_RAY_H
#define GAME_RAY_H


#include "glm/glm.hpp"
using namespace glm;

class Ray {
public:
    Ray(vec3 direction, vec3 position): direction(direction),position(position){};

    vec3 direction;
    vec3 position;
};


#endif //GAME_RAY_H
