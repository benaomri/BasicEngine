#include "RouteBezier.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scene.h"

using namespace std;
using namespace glm;

RouteBezier::RouteBezier()
{
}

void RouteBezier::createRoute3DBezier1D(Scene* scn, int segNum, int res, int mode)
{
    sceneShapes = scn->getShapes();
    scn->AddTexture("../res/textures/box0.bmp", false);
    scn->AddTexture("../res/textures/grass.bmp", false);
    bezier1D = new Bezier1D(segNum, res, mode);

    // Create shapes
    buildAllShapes(scn);
    numberOfSegmentsToDisplay(segNum);

    // Building Bezier 1D
    bezier1D->setBezier1DMesh(bezier1D->GetLine());
    scn->addBezier1DShape(bezier1D, -1);
    scn->SetShapeTex((*sceneShapes).size() - 1, 1);
    scn->MoveCamera(0, Scene::zTranslate, 50);
}

void RouteBezier::animateCubeMovement(bool animate)
{
    if (animate) {
        //Translation
        if (forwardDirection) {
            if (cubeT > 0.99f) {
                cubeSegment++;
                cubeT = 0;
            }
            else {
                cubeT += cubeStepSize;
            }
            if (cubeSegment == bezier1D->GetSegmentsNum()) {
                cubeT = 1;
                cubeSegment--;
                forwardDirection = false;
            }
        }
        else {
            if (cubeT < 0.01f) {
                cubeSegment--;
                cubeT = 1;
            }
            else {
                cubeT -= cubeStepSize;
            }
            if (cubeSegment == -1) {
                cubeT = 0;
                cubeSegment++;
                forwardDirection = true;
            }
        }

        vec4 cubeCenter = (*sceneShapes)[cubeShapeIndex]->getTranslate()[3];
        vec4 nextPosition = bezier1D->GetPointOnCurve(cubeSegment, cubeT);
        vec4 moveVector = nextPosition - cubeCenter;
        (*sceneShapes)[cubeShapeIndex]->MyTranslate(vec3(moveVector.x, moveVector.y, moveVector.z), 0);

        //Rotation
        vec3 currentVelocity = glm::normalize(bezier1D->GetVelosity(cubeSegment, cubeT));
        float angleUpDown = atan2(currentVelocity.y, currentVelocity.x);
        float angleLeftRight = -abs(atan2(currentVelocity.z, currentVelocity.x));
        float delta = sqrt(pow(currentVelocity.x, 2) + pow(currentVelocity.y, 2) + pow(currentVelocity.z, 2));
        vec3 normalUpDown = vec3(0, 0, (currentVelocity.x) / delta);
        vec3 normalLeftRight = vec3(0, currentVelocity.z / delta, 0);
        (*sceneShapes)[cubeShapeIndex]->setRotate(mat4(1));

        if (normalUpDown.z != 0) {
            (*sceneShapes)[cubeShapeIndex]->MyRotate(angleUpDown * (180.f / 3.14f), normalUpDown, 0);
        }

        if (normalLeftRight.y != 0) {
            (*sceneShapes)[cubeShapeIndex]->MyRotate(angleLeftRight * (180.f / 3.14f), normalLeftRight, 0);
        }
    }
}

void RouteBezier::updateCurveByShapes()
{
    vec4 p0 = (*sceneShapes)[0]->getTranslate()[3];
    vec4 p1 = (*sceneShapes)[1]->getTranslate()[3];
    vec4 p2 = (*sceneShapes)[2]->getTranslate()[3];
    vec4 p3 = (*sceneShapes)[3]->getTranslate()[3];
    bezier1D->ChangeFirstSegment(p0, p1, p2, p3);

    int segIndx = 1;
    for (int i = 4; i < sceneShapes->size() - 2; i += 3) {
        vec4 p1 = (*sceneShapes)[i]->getTranslate()[3];
        vec4 p2 = (*sceneShapes)[i + 1]->getTranslate()[3];
        vec4 p3 = (*sceneShapes)[i + 2]->getTranslate()[3];

        bezier1D->ChangeSegment(segIndx, p1, p2, p3);
        segIndx++;
    }
    bezier1D->setBezier1DMesh(bezier1D->GetLine());
}

bool RouteBezier::onCurvePoint(int index)
{
    if (index % 3 == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool RouteBezier::hasLeft(int index)
{
    if (index > firstPointIndex) {
        return true;
    }
    else {
        return false;
    }
}

bool RouteBezier::hasRight(int index)
{
    if (index < lastPointIndex) {
        return true;
    }
    else {
        return false;
    }
}

int RouteBezier::nextShape()
{
    pickedShapeIndex++;
    if (pickedShapeIndex > lastPointIndex) {
        pickedShapeIndex = firstPointIndex;
    }
    return pickedShapeIndex;
}

int RouteBezier::previousShape()
{
    pickedShapeIndex--;
    if (pickedShapeIndex < firstPointIndex) {
        pickedShapeIndex = lastPointIndex;
    }
    return pickedShapeIndex;
}

void RouteBezier::numberOfSegmentsToDisplay(int segNum)
{
    int res = ((bezier1D->getResT() - 1) / bezier1D->GetSegmentsNum() * segNum) + 1;
    int configNum = segNum - 2;

    bezier1D->setResT(res);
    bezier1D->setSegmentsNum(segNum);

    // Reset Cube position
    vec3 cubeNewPosition = bezierConfigurations[configNum][0];
    vec4 cubeOldPosition = (*sceneShapes)[cubeShapeIndex]->getTranslate()[3];
    vec3 movement = cubeNewPosition - vec3(cubeOldPosition.x, cubeOldPosition.y, cubeOldPosition.z);
    (*sceneShapes)[cubeShapeIndex]->MyTranslate(movement, 0);
    (*sceneShapes)[cubeShapeIndex]->setRotate(mat4(1));

    for (int i = 0; i < numberOfOctahedrons; i++) {
        if (i < bezierConfigurations[configNum].size()) {
            (*sceneShapes)[i]->Unhide();

            // Reset Octahedrons posotions
            vec3 pointNewPosition = bezierConfigurations[configNum][i];
            vec4 pointOldPosition = (*sceneShapes)[i]->getTranslate()[3];
            movement = pointNewPosition - vec3(pointOldPosition.x, pointOldPosition.y, pointOldPosition.z);
            (*sceneShapes)[i]->MyTranslate(movement, 0);
        }
        else {
            (*sceneShapes)[i]->Hide();
        }
    }

    lastPointIndex = bezierConfigurations[configNum].size() - 1;
    updateCurveByShapes();
    bezier1D->setBezier1DMesh(bezier1D->GetLine());
}

void RouteBezier::buildAllShapes(Scene* scn)
{
    int shapeIndex = 0;
    firstPointIndex = shapeIndex;

    for (int i = 0; i < numberOfOctahedrons; i++) {
        scn->AddShape(Scene::Octahedron, -1, Scene::TRIANGLES);
        scn->SetShapeTex(shapeIndex, 0);
        (*sceneShapes)[shapeIndex]->MyScale(vec3(0.5, 0.5, 0.5));
        (*sceneShapes)[shapeIndex]->Hide();
        shapeIndex++;
    }

    // Cube
    scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
    scn->SetShapeTex(shapeIndex, 0);

    lastPointIndex = shapeIndex - 1;
    cubeShapeIndex = shapeIndex;

    vec4 zeroVector = vec4(0, 0, 0, 0);
    bezier1D->AddFirstSegment(zeroVector, zeroVector, zeroVector, zeroVector);

    for (int i = 4; i < numberOfOctahedrons; i += 3) {
        bezier1D->AddSegment(zeroVector, zeroVector, zeroVector);
    }

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
    bezierConfigurations.push_back(config2);
    bezierConfigurations.push_back(config3);
    bezierConfigurations.push_back(config4);
    bezierConfigurations.push_back(config5);
    bezierConfigurations.push_back(config6);
}