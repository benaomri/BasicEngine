

#include "Object.h"

void Object::setColor(vec4 color) {
    this->rgb_color = vec3(color.r, color.g, color.b);
    this->shiness = color.a;
}

float Object::getAngle(vec3 hitVec, vec3 normal) {
    // dot product returns cos ==> acos returns angle between the vectors (we need the angle between hitVec and plane)
    // ==> subtracting pi/2 gives us the actual angle ==> division by 2*pi and multiply by 360 gives us an angle in degrees
    return (acos(dot(hitVec, normal)) - acos(.0)) / (4 * acos(.0)) * 360;
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