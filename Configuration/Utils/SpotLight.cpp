#include "SpotLight.h"

SpotLight::SpotLight(vec3 direction) {
    this->liType = Spot;
    this->direction = direction;
}