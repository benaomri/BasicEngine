#include "DirectionalLight.h"
DirectionalLight::DirectionalLight(vec3 lightDirection) {
    this->lightType = Directional;
    this->lightDirection = lightDirection;
}