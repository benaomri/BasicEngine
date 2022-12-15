#include "DirectionalLight.h"
DirectionalLight::DirectionalLight(vec3 direction) {
    this->lightType = Directional;
    this->direction = direction;
}