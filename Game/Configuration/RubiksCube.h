

#ifndef GAME_RUBIKSCUBE_H
#define GAME_RUBIKSCUBE_H

#include "glm/vec4.hpp"
#include "scene.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


#define CUBE_SIZE 1.0
#define NUM_CUBES 3
#define DELTA  (NUM_CUBES-1)/2
#define  ROTATION_SPEED = 10.0f


class RubiksCube {
public:
    RubiksCube();
    void initCubePositions(Scene *scn);
    glm::vec4 angleModulo(glm::vec4 currentAngle);
    void  MoveUp();
    void MoveR();
    void MoveL();
    void MoveU();
    void MoveD();
    void MoveB();
    void MoveF();
    void MoveSPACE();
    void MoveZ();
    void MoveUP();
    void MoveDOWN();
    void MoveLEFT();
    void MoveRIGHT();
    void MoveM();
    void updateStructure(glm::vec3 newPosition);


    // 3D array for storing the positions and indices of the cube faces
    glm::vec4 cubePositions[NUM_CUBES][NUM_CUBES][NUM_CUBES];
    // 3D array for storing the angles and indices of the cube faces
    glm::vec4 cubeAngles[NUM_CUBES][NUM_CUBES][NUM_CUBES];



    std::vector<Shape *> *scnShapes;
    glm::vec3 currentCenter;
    int clockDirection;
    int rotationAngle;

};


#endif //GAME_RUBIKSCUBE_H
