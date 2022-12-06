//
// Created by noamm on 06/12/2022.
//

#include "Hit.h"


Hit::Hit(glm::vec3 hitPoint, Object *obj) {
    this->hitPoint = hitPoint;
    this->obj = obj;
}