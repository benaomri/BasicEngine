#include "Hit.h"

Hit::Hit(vec3 hitPoint, Object *obj) {
    this->hitPoint = hitPoint;
    this->obj = obj;
}