#include "Plane.h"


Plane::Plane(vec4 details, objectTypes type) {
    this->objectDetails = details;
    this->objectType = type;
}

vec3 Plane::normal() {
    return vec3(
            objectDetails.x,
            objectDetails.y,
            objectDetails.z
    );
}

float Plane::distance() {
    return objectDetails.w;
}

float Plane::FindIntersection(Ray ray) {
    vec3 planeNormal = this->normal();
    float a = planeNormal.x;
    float b = planeNormal.y;
    float c = planeNormal.z;
    float d = this->distance();

    float x0 = ray.position.x;
    float y0 = ray.position.y;
    float z0 = ray.position.z;

    float vecx = ray.direction.x;
    float vecy = ray.direction.y;
    float vecz = ray.direction.z;

    float ans = -(a * x0 + b * y0 + c * z0 + d) / (a * vecx + b * vecy + c * vecz);

    return ans;
}

vec3 Plane::getObjectColor(vec3 hitPoint) {
    float scalar = 0.5f;
    float checkers = 0;
    checkers += floor(((hitPoint.x < 0) ? (0.5 - hitPoint.x) : hitPoint.x) / scalar);
    checkers += floor(((hitPoint.y < 0)? (0.5 - hitPoint.y) : hitPoint.y) / scalar);

    checkers = (checkers * 0.5) - int(checkers * 0.5);
    checkers *= 2;
    return (checkers > 0.5) ? 0.5f * this->objectColorRGB : this->objectColorRGB;
}


vec3 Plane::getNormal(vec3 hitPoint) {
    return normalizedVector(normal());
}