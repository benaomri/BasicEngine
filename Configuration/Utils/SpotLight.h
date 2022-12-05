
#ifndef GAME_SPOTLIGHT_H
#define GAME_SPOTLIGHT_H

#include "Light.h"

class SpotLight : public Light {
public:
    SpotLight(vec3 direction);
    /*void setParams(vec3 point, float cosAngle);*/
};


#endif //GAME_SPOTLIGHT_H
