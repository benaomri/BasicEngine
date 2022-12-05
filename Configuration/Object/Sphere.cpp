

#include "Sphere.h"

Sphere::Sphere(vec4 details, objectType objType) {
    this->details = details;
    this->objType = objType;
}

vec3 Sphere::center() {
    return vec3(
            details.x,
            details.y,
            details.z
    );
}

float Sphere::radius() {
    return details.w;
}

float Sphere::FindIntersection(vec3 ray, vec3 somePointOnRay) {
    vec3 center = this->center();
    float mx = center.x;
    float my = center.y;
    float mz = center.z;
    float radius = this->radius();

    float x0 = somePointOnRay.x;
    float y0 = somePointOnRay.y;
    float z0 = somePointOnRay.z;

    float vecx = ray.x;
    float vecy = ray.y;
    float vecz = ray.z;

    vec3 pointMinusCenterVec = somePointOnRay - center;

    //quadratic = vec3(t^2, t, 1)
    vec3 quadratic = vec3(
            1, //pow(vecx, 2) + pow(vecy, 2) + pow(vecz, 2) == 1 (ray is normalized)
            2 * dot(ray, pointMinusCenterVec),
            dot(pointMinusCenterVec, pointMinusCenterVec) - pow(radius, 2)
    );

    float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c

    if (delta < 0.0)
        return INFINITY;

    float root = sqrt(delta);
    float ans1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
    float ans2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a

    return glm::min(ans1, ans2);
}

vec3 Sphere::getColor(vec3 hitPoint) {
    //float angle = this->getAngle(ray, hitPoint);
    return this->rgb_color; // I = I(emission) + K(ambient) * I(ambient) + K(diffuse) * (N dot L) * I(light intensity) + K(specular) * (V dot R)^n * I(light intensity)
}

float Sphere::getAngle(vec3 ray, vec3 hitPoint) {
    vec3 normalToTheSphere = getNormal(hitPoint);
    return Object::getAngle(ray, normalToTheSphere);
}

vec3 Sphere::getNormal(vec3 hitPoint) {
    return normalizedVector(hitPoint - center());
}