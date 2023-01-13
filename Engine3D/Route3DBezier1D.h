#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "scene.h"
#include "BezierCurve.h"

using namespace std;
using namespace glm;

class Route3DBezier1D
{
public:
    // Methods
    Route3DBezier1D();
    void CreateRoute3DBezier1D(Scene* scn, int segNum,int res,int mode);
    void AnimateCubeMovement(bool animate);
    void UpdateCurveByShapes();
    bool OnCurvePoint(int index);
    bool HasLeft(int index);
    bool HasRight(int index);
    int NextShape();
    int PreviousShape();

    // Rebuilding Methods
    void BuildAllShapes(Scene* scn);
    void NumberOfSegmentsToDisplay(int segNum);

    // Variables
    bool C_state = false;
    vector<Shape*>* scn_shapes;
    BezierCurve *bezier_1D;
    int first_point_index, last_point_index;

    // Building 
    int number_of_octahedrons = (6 * 3) + 1; // Max_num_of_segments * 3 + 1
    vector<vector<vec3>> bezier_configs;

    // Animation
    int cube_shape_index;
    bool forward_direction = true;
    int cube_segment = 0;
    float cube_t = 0;
    float cube_step_size = 0.0001;

    // Picking (With arrows)
    bool S_mode = false;
    int picked_shape_index = -1;
};