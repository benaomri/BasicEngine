#ifndef GAME_HIT_H
#define GAME_HIT_H

#include "../Object/Object.h"
#include "glm/glm.hpp"
#include <vector>

using namespace glm;

class Hit {

public:
    Hit(vec3 hitPoint, Object* object): hitPoint(hitPoint),object(object){};
    vec3 hitPoint;
    Object* object;
};




#endif //GAME_HIT_H

