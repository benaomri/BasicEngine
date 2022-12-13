#include "Ray.h"

using namespace glm;
Ray::Ray(vec3 direction, vec3 position) {
    this->position = position;
    this->direction = direction;
}