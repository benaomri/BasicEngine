#include "Light.h"

void Light::setIntensity(vec4 intensity) {
    this->rgbIntensity = vec3(intensity.r, intensity.g, intensity.b);
    this->shiness = intensity.a;
}