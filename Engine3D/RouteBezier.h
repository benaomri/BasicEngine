#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../res/includes/glm/glm.hpp"
#include "../Engine3D/scene.h"
#include "Bezier1D.h"

using namespace std;
using namespace glm;

class RouteBezier
{
public:
    // Functions
    RouteBezier();
    void createRoute3DBezier1D(Scene* scn, int segNum, int res, int mode);
    void animateCubeMovement(bool animate);
    void updateCurveByShapes();
    bool onCurvePoint(int index);
    bool hasLeft(int index);
    bool hasRight(int index);
    int nextShape();
    int previousShape();
    void buildAllShapes(Scene* scn);
    void numberOfSegmentsToDisplay(int segNum);

    // Members
    bool cState = false;
    vector<Shape*>* sceneShapes;
    Bezier1D *bezier1D;
    int firstPointIndex, lastPointIndex;
    int numberOfOctahedrons = (6 * 3) + 1;
    vector<vector<vec3>> bezierConfigurations;
    int cubeShapeIndex;
    bool forwardDirection = true;
    int cubeSegment = 0;
    float cubeT = 0;
    float cubeStepSize; // Get value from main.cpp
    bool sMode = false;
    int pickedShapeIndex = -1;
};