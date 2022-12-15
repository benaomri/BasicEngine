#include "SpotLight.h"

SpotLight::SpotLight(vec3 direction) {
    this->lightType = Spot;
    this->direction = direction;
}