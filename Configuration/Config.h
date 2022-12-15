#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H


#include <string>
#include <vector>
#include "Utils/Image.h"
#include "Object/Object.h"
#include "Light/Light.h"
#include "glm/glm.hpp"
#include "Utils/Hit.h"
#include "Utils/Ray.h"

using namespace std;
using namespace glm;

class Config {
public:

    void readSceneFile(string fileName, int width, int height);
    Ray ConstructRayThroughPixel(int i, int j, int positionOnPixel);
    Hit FindIntersection(Ray ray, int ignoreObjectIndex);
    Image ImageRayCasting();
    vec4 GetColor(Ray ray, Hit hit, int depth);
    vec3 calcDiffuseColor(Hit hit, Light* light);
    vec3 calcSpecularColor(Ray ray, Hit hit, Light* light);
    float calcShadowTerm(Hit hit, Light* light);


    vector<vector<string>> scene_data;

    vec3 eye;
    float bonus_mode_flag;

    vec4 ambient;
    vector<Object *> objects;
    vector<vec4> colors;
    vector<Light *> lights;
    vector<vec4> positions;
    vector<vec4> intensities;

    int image_width, image_height;
    float pixelWidth, pixelHeight;
};


#endif
