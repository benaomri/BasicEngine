#include "Route3DBezier1D.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scene.h"

using namespace std;
using namespace glm;

Route3DBezier1D::Route3DBezier1D()
{
}

// TODO: build 6 segments and display only 3
// Building 3D route with by manipulating 1D Bezier curve
void Route3DBezier1D::CreateRoute3DBezier1D(Scene* scn, int segNum, int res, int mode) {
    scn_shapes = scn->GetShapes();
    int shape_index = 0;

    // Cube + Octahedrons texture
    scn->AddTexture("../res/textures/plane.png", false);

    // Bezier 1D texture
    scn->AddTexture("../res/textures/grass.bmp", false);

    bezier_1D = new BezierCurve(segNum, res, Scene::LINE_STRIP);

    BuildAllShapes(scn);
    NumberOfSegmentsToDisplay(segNum);

    // Building Bezier 1D
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
    scn->AddBezier1DShape(bezier_1D, -1);
    scn->SetShapeTex((*scn_shapes).size() - 1, 1);

    scn->MoveCamera(0, Scene::zTranslate, 50);
}

// TODO: The cube face should change according to the direction of the movement
void Route3DBezier1D::AnimateCubeMovement(bool animate) {
    if (animate) {
        vec3 velocity_before = normalize(bezier_1D->GetVelosity(cube_segment, cube_t));

        if (forward_direction) {
            if (cube_t > 0.99f) {
                cube_segment++;
                cube_t = 0;
            }
            else {
                cube_t += cube_step_size;
            }
            if (cube_segment == bezier_1D->GetSegmentsNum()) {
                cube_t = 1;
                cube_segment--;
                forward_direction = false;
            }
        }
        else {
            if (cube_t < 0.01f) {
                cube_segment--;
                cube_t = 1;
            }
            else {
                cube_t -= cube_step_size;
            }
            if (cube_segment == -1) {
                cube_t = 0;
                cube_segment++;
                forward_direction = true;
            }
        }

        // Translation
        vec4 cube_center = (*scn_shapes)[cube_shape_index]->GetTranslate()[3];
        vec4 next_position = bezier_1D->GetPointOnCurve(cube_segment, cube_t);
        vec4 move_vector = next_position - cube_center;
        (*scn_shapes)[cube_shape_index]->MyTranslate(vec3(move_vector.x, move_vector.y, move_vector.z), 0);

        // Rotation
        vec3 velocity_after = glm::normalize(bezier_1D->GetVelosity(cube_segment, cube_t));
        float angle = glm::dot(velocity_before, velocity_after);
        vec3 normal = glm::cross(velocity_before, velocity_after);

        // Not working yet
        vec3 x = vec3(next_position.x, next_position.y, next_position.z);
        x = normalize(x);
        // x now points at target, unit length
        vec3 y = glm::cross(vec3(0, 0, 1), vec3(x.x, x.y, x.z));
        y = normalize(y);
        // y is now perpendicular to x, unit length
        vec3 z = glm::cross(vec3(x.x, x.y, x.z), y);
        // z is now perpendicular to both x and y, and unit length
        z = normalize(z);

        mat4 matrix;
        matrix[0] = vec4(x.x, x.y, x.z, 0);
        matrix[1] = vec4(y.x, y.y, y.z, 0);
        matrix[2] = vec4(z.x, z.y, z.z, 0);
        matrix[3] = vec4(0, 0, 0, 1);

        bool condition = ((normal.x == 0) && (normal.y == 0) && (normal.z == 0));

        // TODO: Fix cube direction according to line curve
        if (!condition) {
            (*scn_shapes)[cube_shape_index]->MyRotate(angle/11.f, normal, 0);

            //(*scn_shapes)[cube_shape_index]->SetRotate(matrix);
        }
    }
}

void Route3DBezier1D::UpdateCurveByShapes() {
    vec4 p0 = (*scn_shapes)[0]->GetTranslate()[3];
    vec4 p1 = (*scn_shapes)[1]->GetTranslate()[3];
    vec4 p2 = (*scn_shapes)[2]->GetTranslate()[3];
    vec4 p3 = (*scn_shapes)[3]->GetTranslate()[3];
    bezier_1D->ChangeFirstSegment(p0, p1, p2, p3);

    int segIndx = 1;
    for (int i = 4; i < scn_shapes->size() - 2; i += 3) {
        vec4 p1 = (*scn_shapes)[i]->GetTranslate()[3];
        vec4 p2 = (*scn_shapes)[i + 1]->GetTranslate()[3];
        vec4 p3 = (*scn_shapes)[i + 2]->GetTranslate()[3];

        bezier_1D->ChangeSegment(segIndx, p1, p2, p3);
        segIndx++;
    }
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

bool Route3DBezier1D::OnCurvePoint(int index) {
    if (index % 3 == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool Route3DBezier1D::HasLeft(int index) {
    if (index > first_point_index) {
        return true;
    }
    else {
        return false;
    }
}

bool Route3DBezier1D::HasRight(int index) {
    if (index < last_point_index) {
        return true;
    }
    else {
        return false;
    }
}

int Route3DBezier1D::NextShape() {
    picked_shape_index++;
    if (picked_shape_index > last_point_index) {
        picked_shape_index = first_point_index;
    }
    return picked_shape_index;
}
int Route3DBezier1D::PreviousShape() {
    picked_shape_index--;
    if (picked_shape_index < first_point_index) {
        picked_shape_index = last_point_index;
    }
    return picked_shape_index;
}

void Route3DBezier1D::NumberOfSegmentsToDisplay(int segNum) {
    int res = ((bezier_1D->GetResT() - 1) / bezier_1D->GetSegmentsNum() * segNum) + 1;
    bezier_1D->SetSegmentsNum(segNum);
    bezier_1D->SetResT(res);

    int config_num = segNum - 2;

    // Reset Cube position
    vec3 cube_new_position = bezier_configs[config_num][0];
    vec4 cube_old_position = (*scn_shapes)[cube_shape_index]->GetTranslate()[3];
    vec3 movement = cube_new_position - vec3(cube_old_position.x, cube_old_position.y, cube_old_position.z);
    (*scn_shapes)[cube_shape_index]->MyTranslate(movement, 0);

    for (int i = 0; i < number_of_octahedrons; i++) {
        if (i < bezier_configs[config_num].size()) {
            (*scn_shapes)[i]->Unhide();

            // Reset Octahedrons posotions
            vec3 point_new_position = bezier_configs[config_num][i];
            vec4 point_old_position = (*scn_shapes)[i]->GetTranslate()[3];
            vec3 movement = point_new_position - vec3(point_old_position.x, point_old_position.y, point_old_position.z);
            (*scn_shapes)[i]->MyTranslate(movement, 0);
        }
        else {
            (*scn_shapes)[i]->Hide();
        }
    }

    last_point_index = bezier_configs[config_num].size() - 1;
    UpdateCurveByShapes();
    bezier_1D->SetBezier1DMesh(bezier_1D->GetLine());
}

void Route3DBezier1D::BuildAllShapes(Scene* scn) {
    int shape_index = 0;
    first_point_index = shape_index;

    // Octahedrons
    for (int i = 0; i < number_of_octahedrons; i++) {
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shape_index, 0);
        (*scn_shapes)[shape_index]->MyScale(vec3(0.5, 0.5, 0.5));
        (*scn_shapes)[shape_index]->Hide();
        shape_index++;
    }

    // Cube
    scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shape_index, 0);

    last_point_index = shape_index - 1;
    cube_shape_index = shape_index;

    // Setting segments
    vec4 zero_vector = vec4(0, 0, 0, 0);
    bezier_1D->AddFirstSegment(zero_vector, zero_vector, zero_vector, zero_vector);

    for (int i = 4; i < number_of_octahedrons; i += 3)
        bezier_1D->AddSegment(zero_vector, zero_vector, zero_vector);

    // Configurations
    std::vector<glm::vec3> config2;
    config2.resize(7);
    config2[0] = vec3(-9, -9, 0);
    config2[1] = vec3(-9, -3, 0);
    config2[2] = vec3(-6, 0, 0);
    config2[3] = vec3(0, 0, 0);
    config2[4] = vec3(6, 0, 0);
    config2[5] = vec3(9, -3, 0);
    config2[6] = vec3(9, -9, 0);

    std::vector<glm::vec3> config3;
    config3.resize(10);
    config3[0] = vec3(-15, -9, 0);
    config3[1] = vec3(-15, -3, 0);
    config3[2] = vec3(-12, 0, 0);
    config3[3] = vec3(-6, 0, 0);
    config3[4] = vec3(-3, 0, 0);
    config3[5] = vec3(3, 0, 0);
    config3[6] = vec3(6, 0, 0);
    config3[7] = vec3(12, 0, 0);
    config3[8] = vec3(15, -3, 0);
    config3[9] = vec3(15, -9, 0);

    std::vector<glm::vec3> config4;
    config4.resize(13);
    config4[0] = vec3(-21, -9, 0);
    config4[1] = vec3(-21, -3, 0);
    config4[2] = vec3(-18, 0, 0);
    config4[3] = vec3(-12, 0, 0);
    config4[4] = vec3(-9, 0, 0);
    config4[5] = vec3(-3, 0, 0);
    config4[6] = vec3(0, 0, 0);
    config4[7] = vec3(3, 0, 0);
    config4[8] = vec3(9, 0, 0);
    config4[9] = vec3(12, 0, 0);
    config4[10] = vec3(18, 0, 0);
    config4[11] = vec3(21, -3, 0);
    config4[12] = vec3(21, -9, 0);

    std::vector<glm::vec3> config5;
    config5.resize(16);
    config5[0] = vec3(-27, -9, 0);
    config5[1] = vec3(-27, -3, 0);
    config5[2] = vec3(-24, 0, 0);
    config5[3] = vec3(-18, 0, 0);
    config5[4] = vec3(-15, 0, 0);
    config5[5] = vec3(-9, 0, 0);
    config5[6] = vec3(-6, 0, 0);
    config5[7] = vec3(-3, 0, 0);
    config5[8] = vec3(3, 0, 0);
    config5[9] = vec3(6, 0, 0);
    config5[10] = vec3(9, 0, 0);
    config5[11] = vec3(15, 0, 0);
    config5[12] = vec3(18, 0, 0);
    config5[13] = vec3(24, 0, 0);
    config5[14] = vec3(27, -3, 0);
    config5[15] = vec3(27, -9, 0);

    std::vector<glm::vec3> config6;
    config6.resize(19);
    config6[0] = vec3(-33, -9, 0);
    config6[1] = vec3(-33, -3, 0);
    config6[2] = vec3(-30, 0, 0);

    config6[3] = vec3(-24, 0, 0);
    config6[4] = vec3(-21, 0, 0);
    config6[5] = vec3(-15, 0, 0);

    config6[6] = vec3(-12, 0, 0);
    config6[7] = vec3(-9, 0, 0);
    config6[8] = vec3(-3, 0, 0);
    config6[9] = vec3(0, 0, 0);
    config6[10] = vec3(3, 0, 0);
    config6[11] = vec3(9, 0, 0);
    config6[12] = vec3(12, 0, 0);

    config6[13] = vec3(15, 0, 0);
    config6[14] = vec3(21, 0, 0);
    config6[15] = vec3(24, 0, 0);

    config6[16] = vec3(30, 0, 0);
    config6[17] = vec3(33, -3, 0);
    config6[18] = vec3(33, -9, 0);

    // Add all configs
    bezier_configs.push_back(config2);
    bezier_configs.push_back(config3);
    bezier_configs.push_back(config4);
    bezier_configs.push_back(config5);
    bezier_configs.push_back(config6);
}