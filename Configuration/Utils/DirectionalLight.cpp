#include "DirectionalLight.h"
DirectionalLight::DirectionalLight(vec3 direction) {
    this->liType = Directional;
    this->direction = direction;
}