//
// Created by OMRI_PRIVATE on 05/12/2022.
//

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
    float a = planeNormal.x;
    float b = planeNormal.y;
    float c = planeNormal.z;
    float d = this->d();

    float x0 = somePointOnRay.x;
    float y0 = somePointOnRay.y;
    float z0 = somePointOnRay.z;

    float vecx = ray.x;
    float vecy = ray.y;
    float vecz = ray.z;

    //float ans = -(a * x0 + b * y0 + c * z0 + d) / (a * vecx + b * vecy + c * vecz); // = -(dot(planeNormal, somePointOnRay) + d) / dot(planeNormal, ray);
    float ans = INFINITY;
    if (abs(dot(ray, planeNormal)) > 0.0) {
        ans = -(dot(somePointOnRay, planeNormal) + d) / dot(ray, planeNormal);
    }
    return ans;
}

vec3 Plane::getColor(vec3 hitPoint) {
    //float angle = this->getAngle(ray, hitPoint);

    //if ((int(1.5 * hitPoint.x) % 2) == (int(1.5 * hitPoint.y) % 2)) {
    //	return 0.5f * this->rgb_color;
    //}

    //if (((((int(1.5 * hitPoint.x) % 2) == (int(1.5 * hitPoint.y) % 2)) && ((hitPoint.x > 0 && hitPoint.y > 0) || (hitPoint.x < 0 && hitPoint.y < 0)))) ||
    //	((((int(1.5 * hitPoint.x) % 2) != (int(1.5 * hitPoint.y) % 2)) && ((hitPoint.x < 0 && hitPoint.y > 0) || (hitPoint.x > 0 && hitPoint.y < 0))))) {
    //	return 0.5f * this->rgb_color;
    //}

    // Checkerboard pattern
    float scaler_parameter = 0.6f;
    float chessboard = floor((hitPoint.x + 2.0f) / scaler_parameter) + floor((hitPoint.y + 2.0f) / scaler_parameter);
    chessboard = (chessboard * 0.5) - int(chessboard * 0.5);
    chessboard *= 2;
    if (chessboard > 0.5) {
        return 0.5f * this->rgb_color;
    }
    return this->rgb_color; // I = I(emission) + K(ambient) * I(ambient) + K(diffuse) * (N dot L) * I(light intensity) + K(specular) * (V dot R)^n * I(light intensity)
}

float Plane::getAngle(vec3 ray, vec3 hitPoint) {
    vec3 normalToThePlane = getNormal(hitPoint);
    return Object::getAngle(ray, normalToThePlane);
}

vec3 Plane::getNormal(vec3 hitPoint) {
    return normalizedVector(normal());
}