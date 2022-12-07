#include "Sphere.h"

Sphere::Sphere(vec4 details, objectType objType) {
    this->details = details;
    this->objType = objType;
}

vec3 Sphere::center() {
    return vec3(details.x,details.y,details.z);
}

float Sphere::radius() {
    return details.w;
}

float Sphere::FindIntersection(vec3 ray, vec3 somePointOnRay) {
    vec3 center = this->center();
    float radius = this->radius();



    vec3 pointMinusCenterVec = somePointOnRay - center;

    double dotV = dot(pointMinusCenterVec, pointMinusCenterVec) - pow(radius, 2);
    vec3 quadratic = vec3(1,2 * dot(ray, pointMinusCenterVec),dotV);

    float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z;

    if (delta < 0.0)
        return INFINITY;

    float root = sqrt(delta);
    float x1 = (-quadratic.y + root) / (2 * quadratic.x);
    float x2 = (-quadratic.y - root) / (2 * quadratic.x);

    return min(x1, x2);
}

vec3 Sphere::getColor(vec3 hitPoint) {
    return this->rgb_color;
}

float Sphere::getAngle(vec3 ray, vec3 hitPoint) {
    vec3 normalToTheSphere = getNormal(hitPoint);
    return Object::getAngle(ray, normalToTheSphere);
}

vec3 Sphere::getNormal(vec3 hitPoint) {
    return normalizedVector(hitPoint - center());
}