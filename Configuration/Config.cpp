#include "Config.h"
#include "Object/Sphere.h"
#include "Object/Plane.h"
#include "Light/SpotLight.h"
#include "Light/DirectionalLight.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>


void Config::readSceneFile(string fileName, int width, int height) {
    int index = -1;
    string currentLine, currentArgInLine;
    vector<string> sceneLines;
    ifstream sceneFile(fileName);

    while (getline(sceneFile, currentLine)) {
        stringstream textLineStream(currentLine);

        while (getline(textLineStream, currentArgInLine, ' ')) {
            sceneLines.push_back(currentArgInLine);
            index = (index + 1) % 5;

            if (index == 4) {
                sceneData.push_back(sceneLines);
                sceneLines.clear();
            }
        }
    }

    for (int i = 0; i < sceneData.size(); i++) {
        vec4 dataVector = vec4(stof(sceneData[i][1]),
                               stof(sceneData[i][2]),
                               stof(sceneData[i][3]),
                               stof(sceneData[i][4]));

        string lineType = sceneData[i][0];
        // e = eye
        if (lineType == "e") {
            eye = vec3(dataVector.x, dataVector.y, dataVector.z);
            bonusFlag = dataVector.w;
        }
        // a = ambient
        if (lineType == "a")
            ambient = dataVector;


        // d = directional
        if (lineType == "d") {
            dataVector.w > 0 ? lights.push_back(new SpotLight(vec3(dataVector.x, dataVector.y, dataVector.z)))
                             : lights.push_back(
                    new DirectionalLight(vec3(dataVector.x, dataVector.y, dataVector.z)));

        }

        // p = position
        if (lineType == "p")
            positions.push_back(dataVector);


        // i = intensity
        if (lineType == "i")
            intensities.push_back(dataVector);


        // o = object
        if (lineType == "o") {
            dataVector.w > 0 ? objects.push_back(new Sphere(dataVector, Regular))
                             : objects.push_back(new Plane(dataVector, Regular));

        }
        // r = reflective object
        if (lineType == "r") {
            dataVector.w > 0 ? objects.push_back(new Sphere(dataVector, Reflective))
                             : objects.push_back(new Plane(dataVector, Reflective));
        }
        // t = transparent object
        if (lineType == "t") {
            dataVector.w > 0 ? objects.push_back(new Sphere(dataVector, Transparent))
                             : objects.push_back(new Plane(dataVector, Transparent));
        }

        // c = color
        if (lineType == "c")
            colors.push_back(dataVector);

    }

    // Set objects colors
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->objectIndex = i;
        objects[i]->setObjectColor(colors[i]);
    }

    // Set spotlight positions
    int j = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->lightType == Spot) {
            vec3 point = vec3(positions[j].x, positions[j].y, positions[j].z);
            float cosAngle = positions[j++].w;
            lights[i]->lightPosition = point;
            lights[i]->lightCosAngle = cosAngle;
        }
        lights[i]->setIntensity(intensities[i]);
    }

    pixelWidth = 2.0 / float(width);
    pixelHeight = 2.0 / float(height);
    imageWidth = width;
    imageHeight = height;
}


Image Config::ImageRayCasting() {
    Image image = Image(imageWidth, imageHeight);

    // running over the pixels
    for (int j = 0; j < imageHeight; j++) {
        for (int i = 0; i < imageWidth; i++) {
            vec4 pixelColor;


            if (bonusFlag < 1.0) {
                Ray ray = ConstructRayThroughPixel(i, j, 0);
                Hit hit = FindIntersection(ray, -1);
                pixelColor = GetColor(ray, hit, 0);
            }
                // Bonus
            else {
                for (int currentExtraRay = 1; currentExtraRay < 5; currentExtraRay++) {
                    Ray currentRay = ConstructRayThroughPixel(i, j, currentExtraRay);
                    Hit currentHit = FindIntersection(currentRay, -1);
                    vec4 currentPixelColor = GetColor(currentRay, currentHit, 0);
                    pixelColor += currentPixelColor;
                }
                pixelColor = pixelColor / 4.0f;
            }
            image.setColor(i, j, pixelColor);
        }
    }
    return image;
}


Ray Config::ConstructRayThroughPixel(int i, int j, int rayNumber) {
    vec3 topLeftPoint, hitVector, rayDirection;
    // In regular case(center)
    if (rayNumber == 0) {
        topLeftPoint = vec3(-1 + (pixelWidth / 2), 1 - (pixelHeight / 2), 0);
        hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * (j * pixelHeight), 0);
    } else {
        topLeftPoint = vec3(-1 + (pixelWidth / 4), 1 - (pixelHeight / 4), 0);

        // Pixel center top left hit
        if (rayNumber == 1) {
            hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * (j * pixelHeight), 0);
        }
        // Pixel center top right hit
        if (rayNumber == 2) {
            hitVector = topLeftPoint + vec3((i * pixelWidth) + (pixelWidth / 2), -1 * (j * pixelHeight), 0);
        }
        // Pixel center bottom left hit
        if (rayNumber == 3) {
            hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * ((j * pixelHeight) + (pixelHeight / 2)), 0);
        }
        // Pixel center bottom right hit
        if (rayNumber == 4) {
            hitVector = topLeftPoint +
                        vec3((i * pixelWidth) + (pixelWidth / 2), -1 * ((j * pixelHeight) + (pixelHeight / 2)), 0);
        }
    }
    rayDirection = normalizedVector(hitVector - eye);
    Ray ray = Ray(rayDirection, eye);

    return ray;
}


Hit Config::FindIntersection(Ray ray, int ignoreObjectIndex) {
    float minT = INFINITY;
    Object *minPrimitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Space);
    minPrimitive->objectIndex = -1;
    minPrimitive->setObjectColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool gotHit = false;

    for (int i = 0; i < objects.size(); i++) {
        if (i != ignoreObjectIndex) {
            float t = objects[i]->FindIntersection(ray);
            if ((t >= 0) && (t < minT)) {
                gotHit = true;
                minPrimitive = objects[i];
                minT = t;
            }
        }
    }

    return gotHit ? Hit(ray.position + ray.direction * minT, minPrimitive)
                  : Hit(ray.position + ray.direction, minPrimitive);

}

vec4 Config::GetColor(Ray ray, Hit hit, int depth) {
    vec3 phongModelColor = vec3(0, 0, 0);

    // Regular
    if (hit.object->objectType == Regular) {
        vec3 color = hit.object->getObjectColor(hit.hitPoint);
        phongModelColor = color * vec3(ambient.r, ambient.g, ambient.b); // Ambient

        // Checks lights
        for (int i = 0; i < lights.size(); i++) {
            vec3 diffuseColor = max(calcDiffuseColor(hit, lights[i]), vec3(0, 0, 0)); // Diffuse
            vec3 specularColor = max(calcSpecularColor(ray, hit, lights[i]), vec3(0, 0, 0)); // Specular
            float shadowTerm = calcShadowTerm(hit, lights[i]); // Shadow
            phongModelColor += (diffuseColor + specularColor) * shadowTerm;
        }
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }

    // Reflective
    if (hit.object->objectType == Reflective) {
        if (depth == 5) { // MAX_LEVEL=5
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec3 reflection_ray_direction = ray.direction - 2.0f * hit.object->getNormal(hit.hitPoint) *
                                                        dot(ray.direction, hit.object->getNormal(hit.hitPoint));
        Ray reflection_ray = Ray(reflection_ray_direction, hit.hitPoint);

        Hit reflected_hit = FindIntersection(reflection_ray, hit.object->objectIndex);

        if (reflected_hit.object->objectType == Space) {
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec4 reflection_color = GetColor(reflection_ray, reflected_hit, depth + 1);
        phongModelColor = vec3(reflection_color.r, reflection_color.g, reflection_color.b);
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }

    // Transparent
    if (hit.object->objectType == Transparent) {
        if (depth == 5) {
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec4 transparencyColor = vec4(0.f, 0.f, 0.f, 0.f);

        // Transparent Plane
        if (hit.object->objectDetails.w < 0.0) {
            Ray rayThrough = Ray(ray.direction, hit.hitPoint);

            Hit transparency_hit = FindIntersection(rayThrough, hit.object->objectIndex);

            if (transparency_hit.object->objectType == Space) {
                return vec4(0.f, 0.f, 0.f, 0.f);
            }

            transparencyColor = GetColor(rayThrough, transparency_hit, depth + 1);
        }
            // Transparent Sphere
        else {
            // Snell's law
            float pi = 3.14159265;
            float snellFrac = (1.0f / 1.5f);

            float cosFrom = dot(hit.object->getNormal(hit.hitPoint), -ray.direction);
            float thetaFrom = acos(cosFrom) * (180.0f / pi);
            float sinFrom = sin(thetaFrom * (pi / 180.0f));
            float sinTo = snellFrac * sinFrom;
            float thetaTo = asin(sinTo) * (180.0f / pi);
            float cosTo = cos(thetaTo * (pi / 180.0f));

            vec3 rayDirection =
                    (snellFrac * cosFrom - cosTo) * hit.object->getNormal(hit.hitPoint) - snellFrac * (-ray.direction);
            rayDirection = normalizedVector(rayDirection);
            Ray rayIn = Ray(rayDirection, hit.hitPoint);

            Hit transparencyHit = FindIntersection(rayIn, -1);

            if (transparencyHit.object->objectIndex != hit.object->objectIndex) {
                transparencyColor = GetColor(rayIn, transparencyHit, depth + 1);
            }
            else {
                float t = hit.object->FindIntersection(rayIn);
                vec3 secondHitPoint = rayIn.position + rayIn.direction * t;

                cosFrom = dot(-hit.object->getNormal(secondHitPoint), -rayIn.direction);
                thetaFrom = acos(cosFrom) * (180.0f / pi);
                snellFrac = (1.5f / 1.0f);
                sinFrom = sin(thetaFrom * (pi / 180.0f));
                sinTo = snellFrac * sinFrom;
                thetaTo = asin(sinTo) * (180.0f / pi);
                cosTo = cos(thetaTo * (pi / 180.0f));

                vec3 rayOutDirection = (snellFrac * cosFrom - cosTo) * -hit.object->getNormal(hit.hitPoint) -
                                       snellFrac * (-rayIn.direction);
                rayOutDirection = normalizedVector(rayOutDirection);
                Ray ray_out = Ray(rayOutDirection, secondHitPoint);

                Hit transparencyHitOut = FindIntersection(ray_out, hit.object->objectIndex);

                if (transparencyHitOut.object->objectType == Space) {
                    return vec4(0.f, 0.f, 0.f, 0.f);
                }

                transparencyColor = GetColor(ray_out, transparencyHitOut, depth + 1);
            }
        }
        phongModelColor = vec3(transparencyColor.r, transparencyColor.g, transparencyColor.b);
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }
    return vec4(phongModelColor.r, phongModelColor.g, phongModelColor.b, 0.0);
}

vec3 Config::calcDiffuseColor(Hit hit, Light *light) {
    float objectFactor = (hit.object->objectDetails.w < 0.0) ? -1 : 1;
    vec3 normalizedRayDirection = objectFactor * normalizedVector(light->lightDirection);


    // Spotlight special case
    if (light->lightType == Spot) {
        vec3 virtualSpotlightRay = normalizedVector(hit.hitPoint - light->lightPosition);
        float lightCosValue = dot(virtualSpotlightRay, objectFactor * normalizedRayDirection);

        // Checks if the spotlight hit the object
        if (lightCosValue < light->lightCosAngle) {
            return vec3(0.f, 0.f, 0.f);
        }
        normalizedRayDirection = objectFactor * virtualSpotlightRay;

    }
    vec3 objectNormal = hit.object->getNormal(hit.hitPoint);

    // N^*L^ 
    float hitCosValue = dot(objectNormal, -normalizedRayDirection);

    // Kd*(N^*L^)*Il
    vec3 diffuse_color = hit.object->getObjectColor(hit.hitPoint) * hitCosValue * light->lightRgbIntensity;
    return diffuse_color;
}

vec3 Config::calcSpecularColor(Ray ray, Hit hit, Light *light) {
    vec3 normalized_ray_direction = normalizedVector(light->lightDirection);

    // Spotlight
    if (light->lightType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->lightPosition);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        // Checks if the spotlight hit the object
        if (light_cos_value < light->lightCosAngle) {
            return vec3(0.f, 0.f, 0.f);
        }
        normalized_ray_direction = virtual_spotlight_ray;
    }
    vec3 object_normal = hit.object->getNormal(hit.hitPoint);
    vec3 reflected_light_ray =
            normalized_ray_direction - 2.0f * object_normal * dot(normalized_ray_direction, object_normal);
    vec3 ray_to_viewer = normalizedVector(ray.position - hit.hitPoint);

    // V^*R^
    float hitCosValue = dot(ray_to_viewer, reflected_light_ray);
    hitCosValue = glm::max(0.0f, hitCosValue);

    // (V^*R^)^n
    hitCosValue = pow(hitCosValue, hit.object->objectShiness);

    // Ks*(V^*R^)^n*Il
    float Ks = 0.7f;
    vec3 specularColor = Ks * hitCosValue * light->lightRgbIntensity;
    return specularColor;
}

float Config::calcShadowTerm(Hit hit, Light *light) {
    vec3 normalizedRayDirection = normalizedVector(light->lightDirection);
    float minT = INFINITY;

    // Spotlight
    if (light->lightType == Spot) {
        vec3 virtualSpotlightRay = normalizedVector(hit.hitPoint - light->lightPosition);
        float lightCosValue = dot(virtualSpotlightRay, normalizedRayDirection);

        // Checks if hit the object
        if (lightCosValue < light->lightCosAngle) {
            return 0.0;
        }
        normalizedRayDirection = virtualSpotlightRay;
        // Update minT
        minT = -(dot(hit.hitPoint, light->lightPosition)) / abs(dot(-normalizedRayDirection, light->lightPosition));
    }

    // Checks other objects
    for (int i = 0; i < objects.size(); i++) {
        if (i != hit.object->objectIndex) {
            Ray ray = Ray(-normalizedRayDirection, hit.hitPoint);
            float t = objects[i]->FindIntersection(ray);

            if ((t > 0) && (t < minT)) {
                return 0.0;
            }
        }
    }
    return 1.0;
}

