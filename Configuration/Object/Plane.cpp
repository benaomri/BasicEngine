#include "Plane.h"


Plane::Plane(vec4 details, objectType objType) {
    this->details = details;
    this->objType = objType;
}

vec3 Plane::normal() {
    return vec3(
            details.x,
            details.y,
            details.z
    );
}

float Plane::d() {
    return details.w;
}

float Plane::FindIntersection(vec3 ray, vec3 somePointOnRay) {
    vec3 planeNormal = this->normal();
    float d = this->d();
    float ans = INFINITY;
    if (dot(ray, planeNormal) != 0.0) {
        return -(dot(somePointOnRay, planeNormal) + d) / dot(ray, planeNormal);
    }
    return ans;
}

vec3 Plane::getColor(vec3 hitPoint) {
    float scalar = 0.5f;
    float checkers = 0;
    checkers += floor(((hitPoint.x < 0) ? (0.5 - hitPoint.x) : hitPoint.x) / scalar);
    checkers += floor(((hitPoint.y < 0)? (0.5 - hitPoint.y) : hitPoint.y) / scalar);

    checkers = (checkers * 0.5) - int(checkers * 0.5);
    checkers *= 2;
    return (checkers > 0.5) ? 0.5f * this->rgb_color : this->rgb_color;
}

float Plane::getAngle(vec3 ray, vec3 hitPoint) {
    vec3 normalToThePlane = getNormal(hitPoint);
    return Object::getAngle(ray, normalToThePlane);
}

vec3 Plane::getNormal(vec3 hitPoint) {
    return normalizedVector(normal());
}