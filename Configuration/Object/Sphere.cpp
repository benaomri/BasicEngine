#include "Sphere.h"

Sphere::Sphere(vec4 details, objectTypes type) {
    this->objectDetails = details;
    this->objectType = type;
}

vec3 Sphere::center() {
    return vec3(objectDetails.x, objectDetails.y, objectDetails.z);
}

float Sphere::radius() {
    return objectDetails.w;
}

float Sphere::FindIntersection(Ray ray) {
    //TODO check again before submit
    vec3 center = this->center();
    float mx = center.x;
    float my = center.y;
    float mz = center.z;
    float radius = this->radius();

    float x0 = ray.position.x;
    float y0 = ray.position.y;
    float z0 = ray.position.z;

    float vecx = ray.direction.x;
    float vecy = ray.direction.y;
    float vecz = ray.direction.z;

    vec3 pointMinusCenterVec = ray.position - center;

    //quadratic = vec3(t^2, t, 1)
    vec3 quadratic = vec3(
            1, //pow(vecx, 2) + pow(vecy, 2) + pow(vecz, 2) == 1 (ray is normalized)
            2 * dot(ray.direction, pointMinusCenterVec),
            dot(pointMinusCenterVec, pointMinusCenterVec) - pow(radius, 2)
    );

    float delta = pow(quadratic.y, 2) - 4 * quadratic.x * quadratic.z; // b^2-4*a*c

    if (delta < 0.0)
        return INFINITY;

    float root = sqrt(delta);
    float ans1 = (-quadratic.y + root) / (2 * quadratic.x); // (-b + root) / 2*a
    float ans2 = (-quadratic.y - root) / (2 * quadratic.x); // (-b - root) / 2*a
    float result = glm::min(ans1, ans2);

    // In case of Transperant spheres
    float threshold = 0.001f;
    if (result <= threshold) {
        result = glm::max(ans1, ans2);
    }

    return result;
}

vec3 Sphere::getObjectColor(vec3 hitPoint) {
    return this->objectColorRGB;
}

vec3 Sphere::getNormal(vec3 hitPoint) {
    return normalizedVector(hitPoint - center());
}