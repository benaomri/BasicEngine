#include "Object.h"

void Object::setColor(vec4 color) {
    this->rgb_color = vec3(color.r, color.g, color.b);
    this->shiness = color.a;
}

float vectorSize(vec3 vec) {
    return sqrt(powf(vec.x, 2.) + powf(vec.y, 2.) + powf(vec.z, 2.));
}
vec3 normalizedVector(vec3 vec) {
    float vecSize = vectorSize(vec);
    vec3 normalized = vec3(vec);
    normalized.x /= vecSize;
    normalized.y /= vecSize;
    normalized.z /= vecSize;
    return normalized;
}