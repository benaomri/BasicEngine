#ifndef GAME_RUBIKSCUBE_H
#define GAME_RUBIKSCUBE_H

#include "glm/vec4.hpp"
#include "scene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define NUM_CUBES 3

class RubiksCube {
public:
    RubiksCube();
    void initCubePositions(Scene *scn);
    glm::vec3 angleModulo(glm::vec3 currentAngle);
    void MoveR();
    void MoveL();
    void MoveU();
    void MoveD();
    void MoveB();
    void MoveF();
    void MoveSPACE();
    void MoveZ();
    void MoveA();
    void MoveUP();
    void MoveDOWN();
    void MoveLEFT();
    void MoveRIGHT();
    void updateStructure();
    void rotateCube(int x, int y, int z, glm::vec3 rotationDirection);

    std::vector<std::vector<std::vector<std::pair<int, glm::vec3>>>>  cubePositions;
    std::vector<std::vector<std::vector<std::pair<int, glm::vec3>>>>  cubeAngles;

    std::vector<Shape *> *shapes;
    glm::vec3 currentCenter;
    int clockDirection;
    int rotationAngle;

};


#endif
