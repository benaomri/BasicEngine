

#ifndef GAME_DIRECTIONALLIGHT_H
#define GAME_DIRECTIONALLIGHT_H

#include "Light.h"

class DirectionalLight : public Light{
public:
    DirectionalLight(vec3 direction);
    /*virtual void setParams(vec3 point, float cosAngle);*/
};


#endif //GAME_DIRECTIONALLIGHT_H
