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

void Config::read_file(string file_name, int width, int height) {
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
        vec4 input_vector = vec4(stof(scene_data[i][1]),
                                 stof(scene_data[i][2]),
                                 stof(scene_data[i][3]),
                                 stof(scene_data[i][4]));

        // e = eye
        if (scene_data[i][0] == "e") {
            eye = vec3(input_vector.x, input_vector.y, input_vector.z);
            bonus_mode_flag = input_vector.w;
        }
        // a = ambient
        if (scene_data[i][0] == "a")
            ambient = input_vector;


        // d = directional
        if (scene_data[i][0] == "d") {
            input_vector.w > 0 ? lights.push_back(new SpotLight(vec3(input_vector.x, input_vector.y, input_vector.z)))
                               : lights.push_back(
                    new DirectionalLight(vec3(input_vector.x, input_vector.y, input_vector.z)));

        }

        // p = position
        if (scene_data[i][0] == "p")
            positions.push_back(input_vector);


        // i = intensity
        if (scene_data[i][0] == "i")
            intensities.push_back(input_vector);


        // o = object
        if (scene_data[i][0] == "o") {
            input_vector.w > 0 ? objects.push_back(new Sphere(input_vector, Regular))
                               : objects.push_back(new Plane(input_vector, Regular));

        }
        // r = reflective object
        if (scene_data[i][0] == "r") {
            input_vector.w > 0 ? objects.push_back(new Sphere(input_vector, Reflective))
                               : objects.push_back(new Plane(input_vector, Reflective));
        }
        // t = transparent object
        if (scene_data[i][0] == "t") {
            input_vector.w > 0 ? objects.push_back(new Sphere(input_vector, Transparent))
                               : objects.push_back(new Plane(input_vector, Transparent));
        }

        // c = color
        if (scene_data[i][0] == "c")
            colors.push_back(input_vector);

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

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
            if ((i == 750) && (j == 500)) {
                cout << "TEST" << endl;
            }
            vec3 ray = ConstructRayThroughPixel(i, j);
            Hit hit = FindIntersection(ray, eye);
            vec4 pixel_color = GetColor(ray, hit, eye, 0);

            image.setColor(i, j, pixel_color);
        }
    }
    return image;
}


vec3 Config::ConstructRayThroughPixel(int i, int j) {
    vec3 top_left_point = vec3(-1 + (pixel_width / 2), 1 - (pixel_height / 2), 0);
    vec3 hit_on_screen = top_left_point + vec3(i * pixel_width, -1 * (j * pixel_height), 0);
    vec3 ray_direction = hit_on_screen - eye;

    return normalizedVector(ray_direction);
}


Hit Config::FindIntersection(vec3 ray, vec3 starting_ray) {
    float min_t = INFINITY;
    Object *min_primitive = new Plane(vec4(1.0, 1.0, 1.0, 1.0), Space);
    min_primitive->setColor(vec4(0.0, 0.0, 0.0, 0.0));
    bool got_hit = false;

    for (int i = 0; i < objects.size(); i++) {
        float t = objects[i]->FindIntersection(ray, starting_ray);
        if ((t > 0) && (t < min_t)) {
            got_hit = true;
            min_primitive = objects[i];
            min_t = t;
        }
    }

    return got_hit ? Hit(eye + ray * min_t, min_primitive)
                   : Hit(eye + ray, min_primitive);

}

vec4 Config::GetColor(vec3 ray, Hit hit, vec3 starting_ray, int level) {
    vec3 color = hit.obj->getColor(hit.hitPoint);
    vec3 phong_model_color = color * vec3(ambient.r, ambient.g, ambient.b); // Ambient

    for (int i = 0; i < lights.size(); i++) {
        vec3 diffuse_color = max(calcDiffuseColor(hit, lights[i]), vec3(0, 0, 0)); // Diffuse
        vec3 specular_color = max(calcSpecularColor(hit, lights[i], starting_ray), vec3(0, 0, 0)); // Specular
        float shadow_term = calcShadowTerm(hit, lights[i]);

        phong_model_color += (diffuse_color + specular_color) * shadow_term;
    }
    phong_model_color = min(phong_model_color, vec3(1.0, 1.0, 1.0));

    if (hit.obj->objType == Reflective) {
        if (level == 1) { // MAX_LEVEL=1
            return vec4(0, 0, 0, 0);
        }
        vec3 reflection_ray =
                ray - 2.0f * hit.obj->getNormal(hit.hitPoint) * dot(ray, hit.obj->getNormal(hit.hitPoint));
        Hit reflected_hit = FindIntersection(reflection_ray, hit.hitPoint);

        if (reflected_hit.obj->objType == Space) {
            return vec4(0, 0, 0, 0);
        }

        vec4 reflection_color = GetColor(reflection_ray, reflected_hit, hit.hitPoint, level + 1);
        phong_model_color = vec3(starting_ray.r, starting_ray.g, reflection_color.b);
        phong_model_color = min(phong_model_color, vec3(1.0, 1.0, 1.0));
    }

    return vec4(phong_model_color.r, phong_model_color.g, phong_model_color.b, 0.0);
}

vec3 Config::calcDiffuseColor(Hit hit, Light *light) {
    float object_factor = (hit.obj->details.w < 0.0 ) ? -1 : 1;

    vec3 normalized_ray_direction = object_factor * normalizedVector(light->direction);

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, object_factor * normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return vec3(0.0, 0.0, 0.0);
        } else {
            normalized_ray_direction = object_factor * virtual_spotlight_ray;
        }
    }
    vec3 object_normal = hit.obj->getNormal(hit.hitPoint);

    // N^*L^ = cos(t)
    float hit_cos_value = dot(object_normal, -normalized_ray_direction);

    // Id = Kd*(N^*L^)*Il
    vec3 diffuse_color = hit.obj->getColor(hit.hitPoint) * hit_cos_value * light->rgb_intensity;
    return diffuse_color;
}

vec3 Config::calcSpecularColor(Hit hit, Light *light, vec3 starting_ray) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return vec3(0.0, 0.0, 0.0);
        } else {
            normalized_ray_direction = virtual_spotlight_ray;
        }
    }
    vec3 object_normal = hit.obj->getNormal(hit.hitPoint);
    vec3 reflected_light_ray =
            normalized_ray_direction - 2.0f * object_normal * dot(normalized_ray_direction, object_normal);
    vec3 ray_to_viewer = normalizedVector(starting_ray - hit.hitPoint);

    // V^*R^ = max(0, V^*R^)
    float hit_cos_value = dot(ray_to_viewer, reflected_light_ray);
    hit_cos_value = glm::max(0.0f, hit_cos_value);
    // (V^*R^)^n
    hit_cos_value = pow(hit_cos_value, hit.obj->shiness);

    // Id = Ks*(V^*R^)^n*Il
    // Ks = (0.7, 0.7, 0.7)
    vec3 speculat_color = 0.7f * hit_cos_value * light->rgb_intensity;
    return speculat_color;
}

float Config::calcShadowTerm(Hit hit, Light *light) {
    vec3 normalized_ray_direction = normalizedVector(light->direction);
    float min_t = INFINITY;

    if (light->liType == Spot) {
        vec3 virtual_spotlight_ray = normalizedVector(hit.hitPoint - light->position);
        float light_cos_value = dot(virtual_spotlight_ray, normalized_ray_direction);

        if (light_cos_value < light->cosAngle) {
            return 0.0;
        } else {
            normalized_ray_direction = virtual_spotlight_ray;
            // Update min_t
            min_t = -(dot(hit.hitPoint, light->position)) / abs(dot(-normalized_ray_direction, light->position));
        }
    }

    for (int i = 0; i < objects.size(); i++) {
        if (i != hit.obj->objIndex) {
            float t = objects[i]->FindIntersection(-normalized_ray_direction, hit.hitPoint);

            if ((t > 0) && (t < min_t)) {
                return 0.0;
            }
        }
    }
    return 1.0;
}

