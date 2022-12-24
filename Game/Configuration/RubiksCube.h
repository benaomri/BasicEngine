

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
    glm::vec3 angleModulo(glm::vec3 currentAngle);
    void  MoveUp();
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
    void MoveM();
    void updateStructure();
    void Rotate_Cube(int i, int j, int k, glm::vec3 rotation_direction);



        // 3D array for storing the positions and indices of the cube faces
    std::vector<std::vector<std::vector<std::pair<int, glm::vec3>>>>  cubePositions;
    // 3D array for storing the angles and indices of the cube faces
    std::vector<std::vector<std::vector<std::pair<int, glm::vec3>>>>  cubeAngles;



    std::vector<Shape *> *scnShapes;
    glm::vec3 currentCenter;
    int clockDirection;
    int rotationAngle;

//    int rotation_per_frame;
//
//    bool unlocked;
//    bool activate_animation;
//    bool animating;
//    char action;
//    int num_of_actions;

};


#endif //GAME_RUBIKSCUBE_H
