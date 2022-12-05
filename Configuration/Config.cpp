//
// Created by OMRI_PRIVATE on 05/12/2022.
//

#include "Config.h"
#include "Object/Sphere.h"
#include "Object/Plane.h"
#include "Utils/SpotLight.h"
#include "Utils/DirectionalLight.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;



void Config::read_file(std::string file_name, int width, int height) {
    int index = -1;
    string text_line, text_argument;
    vector<string> scene_line;
    ifstream scene_file(file_name);

    cout << "File data:" << endl;

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(scene_file, text_line)) {
        cout << text_line << endl;

        std::stringstream text_line_stream(text_line);

        while (getline(text_line_stream, text_argument, ' ')) {
            scene_line.push_back(text_argument);
            index = (index + 1) % 5;

            if (index == 4) {
                scene_data.push_back(scene_line);
                scene_line.clear();
            }
        }
    }

    for (int i = 0; i < scene_data.size(); i++) {
        vec4 input_vector = vec4(stof(scene_data[i][1]), stof(scene_data[i][2]), stof(scene_data[i][3]),
                                 stof(scene_data[i][4]));

        // e = eye - XYZ + Bonus Mode Flag
        if (scene_data[i][0] == "e") {
            eye = vec3(input_vector.x, input_vector.y, input_vector.z);
            bonus_mode_flag = input_vector.w;
        }
        // a = ambient - RGBA
        if (scene_data[i][0] == "a") {
            ambient = input_vector;
        }
        // r = reflective object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "r") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Reflective));
            else objects.push_back(new Plane(input_vector, Reflective));
        }
        // t = transparent object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "t") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Transparent));
            else objects.push_back(new Plane(input_vector, Transparent));
        }
        // o = object - XYZW (W>0 -> Spheres) / (W<0 -> Planes)
        if (scene_data[i][0] == "o") {
            if (input_vector.w > 0)
                objects.push_back(new Sphere(input_vector, Regular));
            else objects.push_back(new Plane(input_vector, Regular));
        }
        // c = color (ambient and diffuse of object) - RGBA (A -> Shininess)
        if (scene_data[i][0] == "c") {
            colors.push_back(input_vector);
        }
        // d = direction (of light sources) - XYZW
        if (scene_data[i][0] == "d") {
            if (input_vector.w > 0)
                lights.push_back(new SpotLight(vec3(input_vector.x, input_vector.y, input_vector.z)));
            else lights.push_back(new DirectionalLight(vec3(input_vector.x, input_vector.y, input_vector.z)));
        }
        // p = position (of spotlights only) - XYZW
        if (scene_data[i][0] == "p") {
            positions.push_back(input_vector);
        }
        // i = intensity (of light sources) - RGBA
        if (scene_data[i][0] == "i") {
            intensities.push_back(input_vector);
        }
    }

    // Set objects colors
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->objIndex = i;
        objects[i]->setColor(colors[i]);
    }

    // Set spotlight positions
    int j = 0;
    for (int i = 0; i < lights.size(); i++) {
        if (lights[i]->liType == Spot) {
            vec3 point = vec3(positions[j].x, positions[j].y, positions[j].z);
            float cosAngle = positions[j++].w;
            lights[i]->position = point;
            lights[i]->cosAngle = cosAngle;
        }
        lights[i]->setIntensity(intensities[i]);
    }

    // Finding pixel width and height and saving parameters
    pixel_width = 2.0 / float(width);
    pixel_height = 2.0 / float(height);
    image_width = width;
    image_height = height;
}



Image Config::ImageRayCasting() {
    Image image = Image(image_width, image_height);
//
//    for (int j = 0; j < image_height; j++) {
//        for (int i = 0; i < image_width; i++) {
//            if ((i == 200) && (j == 600)) {
//                cout << "TEST" << endl;
//            }
//            vec3 ray = ConstructRayThroughPixel(i, j);
//            Hit hit = FindIntersection(ray);
//            vec4 pixel_color = GetColor(ray, hit);
//
//            image.setColor(i, j, pixel_color);
//        }
//    }
    return image;
}
