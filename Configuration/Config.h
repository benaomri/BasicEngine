#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H


#include <string>
#include <vector>
#include "Utils/Image.h"
#include "Object/Object.h"
#include "Utils/Light.h"
#include "glm/glm.hpp"
#include "Utils/Hit.h"


using namespace std;
using namespace glm;

class Config {
public:

    void read_file(string file_name, int width, int height);
    vec3 ConstructRayThroughPixel(int i, int j);
    Hit FindIntersection(vec3 ray);
    Image ImageRayCasting();
    vec4 GetColor(vec3 ray, Hit hit, int level);
    vec3 calcDiffuseColor(Hit hit, Light* light);
    vec3 calcSpecularColor(Hit hit, Light* light);
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
    float pixel_width, pixel_height;

    vec3 forward_vector;
    vec3 up_vector;
    vec3 right_vector;
    vec2 center_dot;


};


#endif
