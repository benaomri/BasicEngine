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

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(sceneFile, currentLine)) {
       stringstream textLineStream(currentLine);

        while (getline(textLineStream, currentArgInLine, ' ')) {
            sceneLines.push_back(currentArgInLine);
            index = (index + 1) % 5;

            if (index == 4) {
                scene_data.push_back(sceneLines);
                sceneLines.clear();
            }
        }
    }

    for (int i = 0; i < scene_data.size(); i++) {
        vec4 dataVector = vec4(stof(scene_data[i][1]),
                               stof(scene_data[i][2]),
                               stof(scene_data[i][3]),
                               stof(scene_data[i][4]));

        string lineType = scene_data[i][0];
        // e = eye
        if ( lineType == "e") {
            eye = vec3(dataVector.x, dataVector.y, dataVector.z);
            bonus_mode_flag = dataVector.w;
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
        objects[i]->objIndex = i;
        objects[i]->setColor(colors[i]);
    }

    // Set spotlight positions
    int j = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->lightType == Spot) {
            vec3 point = vec3(positions[j].x, positions[j].y, positions[j].z);
            float cosAngle = positions[j++].w;
            lights[i]->position = point;
            lights[i]->cosAngle = cosAngle;
        }
        lights[i]->setIntensity(intensities[i]);
    }

    // Finding pixel width and height and saving parameters
    pixelWidth = 2.0 / float(width);
    pixelHeight = 2.0 / float(height);
    image_width = width;
    image_height = height;
}


Image Config::ImageRayCasting() {
    Image image = Image(image_width, image_height);

    // Looping over all the pixels
    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            vec4 pixelColor;

            //TODO drop Bonus cause who we are laying for?
            // Single Sampling
            if (bonus_mode_flag < 0.5) {
                Ray ray = ConstructRayThroughPixel(i, j, 0);
                Hit hit = FindIntersection(ray, -1);
                pixelColor = GetColor(ray, hit, 0);
            }
                // Multi Sampling (Bonus)
            else {
                Ray ray1 = ConstructRayThroughPixel(i, j, 1);
                Ray ray2 = ConstructRayThroughPixel(i, j, 2);
                Ray ray3 = ConstructRayThroughPixel(i, j, 3);
                Ray ray4 = ConstructRayThroughPixel(i, j, 4);
                Hit hit1 = FindIntersection(ray1, -1);
                Hit hit2 = FindIntersection(ray2, -1);
                Hit hit3 = FindIntersection(ray3, -1);
                Hit hit4 = FindIntersection(ray4, -1);
                vec4 pixel_color1 = GetColor(ray1, hit1, 0);
                vec4 pixel_color2 = GetColor(ray2, hit2, 0);
                vec4 pixel_color3 = GetColor(ray3, hit3, 0);
                vec4 pixel_color4 = GetColor(ray4, hit4, 0);
                pixelColor = (pixel_color1 + pixel_color2 + pixel_color3 + pixel_color4) / 4.0f;
            }
            image.setColor(i, j, pixelColor);
        }
    }
    return image;
}


Ray Config::ConstructRayThroughPixel(int i, int j, int positionOnPixel) {
    vec3 topLeftPoint, hitVector, rayDirection;

    // Pixel center hit
    if (positionOnPixel == 0) {
        topLeftPoint = vec3(-1 + (pixelWidth / 2), 1 - (pixelHeight / 2), 0);
        hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * (j * pixelHeight), 0);
    }
    else {
        topLeftPoint = vec3(-1 + (pixelWidth / 4), 1 - (pixelHeight / 4), 0);

        // Pixel center top left hit
        if (positionOnPixel == 1) {
            hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * (j * pixelHeight), 0);
        }
        // Pixel center top right hit
        if (positionOnPixel == 2) {
            hitVector = topLeftPoint + vec3((i * pixelWidth) + (pixelWidth / 2), -1 * (j * pixelHeight), 0);
        }
        // Pixel center bottom left hit
        if (positionOnPixel == 3) {
            hitVector = topLeftPoint + vec3(i * pixelWidth, -1 * ((j * pixelHeight) + (pixelHeight / 2)), 0);
        }
        // Pixel center bottom right hit
        if (positionOnPixel == 4) {
            hitVector = topLeftPoint + vec3((i * pixelWidth) + (pixelWidth / 2), -1 * ((j * pixelHeight) + (pixelHeight / 2)), 0);
        }
    }
    rayDirection = normalizedVector(hitVector - eye);
    Ray ray = Ray(rayDirection, eye);

    return ray;
}


Hit Config::FindIntersection(Ray ray, int ignoreObjectIndex) {
    float minT = INFINITY;
    Object *minPrimitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Space);
    minPrimitive->objIndex = -1;
    minPrimitive->setColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool gotHit = false;

    for (int i = 0; i < objects.size(); i++) {
        if(i != ignoreObjectIndex){//Todo all the time -1
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

    // Regular case
    if (hit.obj->objType == Regular) {
        vec3 color = hit.obj->getColor(hit.hitPoint);
        phongModelColor = color * vec3(ambient.r, ambient.g, ambient.b); // Ambient

        // Looping over all the light sources
        for (int i = 0; i < lights.size(); i++) {
            vec3 diffuseColor = max(calcDiffuseColor(hit, lights[i]), vec3(0, 0, 0)); // Diffuse
            vec3 specularColor = max(calcSpecularColor(ray, hit, lights[i]), vec3(0, 0, 0)); // Specular
            float shadowTerm = calcShadowTerm(hit, lights[i]); // Shadow

            // I = I(emission) + K(ambient) * I(ambient) + (K(diffuse) * dot(N,L) * I(light intensity) + K(specular) * dot(V,R)^n * I(light intensity)) * S * I(shadow)
            phongModelColor += (diffuseColor + specularColor) * shadowTerm;
        }
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }

    // Reflective case
    if (hit.obj->objType == Reflective) {
        if (depth == 5) { // MAX_LEVEL=5
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec3 reflection_ray_direction = ray.direction - 2.0f * hit.obj->getNormal(hit.hitPoint) * dot(ray.direction, hit.obj->getNormal(hit.hitPoint));
        Ray reflection_ray = Ray(reflection_ray_direction, hit.hitPoint);

        Hit reflected_hit = FindIntersection(reflection_ray, hit.obj->objIndex);

        if (reflected_hit.obj->objType == Space) {
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec4 reflection_color = GetColor(reflection_ray, reflected_hit, depth + 1);

        // The Correct, but less beautiful equation
        //phongModelColor = 0.7f * vec3(reflection_color.r, reflection_color.g, reflection_color.b);

        phongModelColor = vec3(reflection_color.r, reflection_color.g, reflection_color.b);
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }

    // Transparent case
    if (hit.obj->objType == Transparent) {
        if (depth == 5) { // MAX_LEVEL=5
            return vec4(0.f, 0.f, 0.f, 0.f);
        }

        vec4 transparencyColor = vec4(0.f, 0.f, 0.f, 0.f);

        // Transparent Plane
        if (hit.obj->details.w < 0.0) {
            Ray rayThrough = Ray(ray.direction, hit.hitPoint);

            Hit transparency_hit = FindIntersection(rayThrough, hit.obj->objIndex);

            if (transparency_hit.obj->objType == Space) {
                return vec4(0.f, 0.f, 0.f, 0.f);
            }

            transparencyColor = GetColor(rayThrough, transparency_hit, depth + 1);
        }
            // Transparent Sphere
        else {
            // Snell's law
            float pi = 3.14159265;
            float snellFrac = (1.0f / 1.5f);

            float cosFrom = dot(hit.obj->getNormal(hit.hitPoint), -ray.direction);
            float thetaFrom = acos(cosFrom) * (180.0f / pi);
            float sinFrom = sin(thetaFrom * (pi / 180.0f));
            float sinTo = snellFrac * sinFrom;
            float thetaTo = asin(sinTo) * (180.0f / pi);
            float cosTo = cos(thetaTo * (pi / 180.0f));

            // Finding the second hit inside the sphere
            vec3 rayDriection = (snellFrac * cosFrom - cosTo) * hit.obj->getNormal(hit.hitPoint) - snellFrac * (-ray.direction);
            rayDriection = normalizedVector(rayDriection);
            Ray rayIn = Ray(rayDriection, hit.hitPoint);

            Hit transparencyHit = FindIntersection(rayIn, -1);

            // Second hit inside the sphere to other object
            if (transparencyHit.obj->objIndex != hit.obj->objIndex) {
                transparencyColor = GetColor(rayIn, transparencyHit, depth + 1);
            }
                // Second hit inside the sphere to outside
            else {
                float t = hit.obj->FindIntersection(rayIn);
                vec3 secondHitPoint = rayIn.position + rayIn.direction * t;

                // Reverse calculations
                cosFrom = dot(-hit.obj->getNormal(secondHitPoint), -rayIn.direction);
                thetaFrom = acos(cosFrom) * (180.0f / pi);
                snellFrac = (1.5f / 1.0f);
                sinFrom = sin(thetaFrom * (pi / 180.0f));
                sinTo = snellFrac * sinFrom;
                thetaTo = asin(sinTo) * (180.0f / pi);
                cosTo = cos(thetaTo * (pi / 180.0f));

                // Finding the ray out of the sphere
                vec3 rayOutDirection = (snellFrac * cosFrom - cosTo) * -hit.obj->getNormal(hit.hitPoint) - snellFrac * (-rayIn.direction);
                rayOutDirection = normalizedVector(rayOutDirection);
                Ray ray_out = Ray(rayOutDirection, secondHitPoint);

                Hit transparencyHitOut = FindIntersection(ray_out, hit.obj->objIndex);

                if (transparencyHitOut.obj->objType == Space) {
                    return vec4(0.f, 0.f, 0.f, 0.f);
                }

                transparencyColor = GetColor(ray_out, transparencyHitOut, depth + 1);
            }
        }

        // The Correct, but less beautiful equation
        //phongModelColor = 0.7f * vec3(transparencyColor.r, transparencyColor.g, transparencyColor.b);

        phongModelColor = vec3(transparencyColor.r, transparencyColor.g, transparencyColor.b);
        phongModelColor = min(phongModelColor, vec3(1.0, 1.0, 1.0));
    }
    return vec4(phongModelColor.r, phongModelColor.g, phongModelColor.b, 0.0);
}

vec3 Config::calcDiffuseColor(Hit hit, Light *light) {
    // Planes normals are in the opsite direction to the viewer than Spheres

    float objectFactor = (hit.obj->details.w < 0.0)? -1 : 1;
    vec3 normalizedRayDirection = objectFactor * normalizedVector(light->direction);


    // Spotlight special case
    if (light->lightType == Spot) {
        vec3 virtualSpotlightRay = normalizedVector(hit.hitPoint - light->position);
        float lightCosValue = dot(virtualSpotlightRay, objectFactor * normalizedRayDirection);

        // Checking if the spotlight rays hit the object
        if (lightCosValue < light->cosAngle) {
            return vec3(0.f, 0.f, 0.f);
        }
        else {
            normalizedRayDirection = objectFactor * virtualSpotlightRay;
        }
    }
    vec3 objectNormal = hit.obj->getNormal(hit.hitPoint);

    // N^*L^ = cos(t)
    float hitCosValue = dot(objectNormal, -normalizedRayDirection);

    // Id = Kd*(N^*L^)*Il
    vec3 diffuse_color = hit.obj->getColor(hit.hitPoint) * hitCosValue * light->rgb_intensity;
    return diffuse_color;
}

vec3 Config::calcSpecularColor(Ray ray, Hit hit, Light *light) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);

    // Spotlight special case
    if (light->lightType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        // Checking if the spotlight rays hit the object
        if (light_cos_value < light->cosAngle) {
            return vec3(0.f, 0.f, 0.f);
        }
        else {
            normalized_ray_direction = virtual_spotlight_ray;
        }
    }
    vec3 object_normal = hit.obj->getNormal(hit.hitPoint);
    vec3 reflected_light_ray = normalized_ray_direction - 2.0f * object_normal * dot(normalized_ray_direction, object_normal);
    vec3 ray_to_viewer = normalizedVector(ray.position - hit.hitPoint);

    // V^*R^ = max(0, V^*R^)
    float hitCosValue = dot(ray_to_viewer, reflected_light_ray);
    hitCosValue = glm::max(0.0f, hitCosValue);

    // (V^*R^)^n
    hitCosValue = pow(hitCosValue, hit.obj->shiness);

    // Id = Ks*(V^*R^)^n*Il
    // Ks = (0.7, 0.7, 0.7)
    vec3 speculatColor = 0.7f * hitCosValue * light->rgb_intensity;
    return speculatColor;
}

float Config::calcShadowTerm(Hit hit, Light *light) {
    vec3 normalizedRayDirection = normalizedVector(light->direction);
    float minT = INFINITY;

    // Spotlight special case
    if (light->lightType == Spot) {
        vec3 virtualSpotlightRay = normalizedVector(hit.hitPoint - light->position);
        float lightCosValue = dot(virtualSpotlightRay, normalizedRayDirection);

        // Checking if the spotlight rays hit the object
        if (lightCosValue < light->cosAngle) {
            return 0.0;
        }
        else {
            normalizedRayDirection = virtualSpotlightRay;
            // Update minT to the value of the light position
            minT = -(dot(hit.hitPoint, light->position)) / abs(dot(-normalizedRayDirection, light->position));
        }
    }

    // Checking the path between the light source and the object by looping over all the objects
    for (int i = 0; i < objects.size(); i++) {
        if (i != hit.obj->objIndex) {
            Ray ray = Ray(-normalizedRayDirection, hit.hitPoint);
            float t = objects[i]->FindIntersection(ray);

            if ((t > 0) && (t < minT)) {
                return 0.0;
            }
        }
    }
    return 1.0;
}

