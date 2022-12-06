//
// Created by noamm on 06/12/2022.
//

#ifndef GAME_HIT_H
#define GAME_HIT_H


#include "../Object/Object.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;

class Hit {

public:
    vec3 hitPoint;
    Object* obj;
    Hit(vec3 hitPoint, Object* obj);
};




#endif //GAME_HIT_H

