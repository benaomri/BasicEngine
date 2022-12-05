
#include "Light.h"


void Light::setIntensity(vec4 intensity) {
    this->rgb_intensity = vec3(intensity.r, intensity.g, intensity.b);
    this->shiness = intensity.a;
}