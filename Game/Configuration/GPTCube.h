

#ifndef GAME_GPTCUBE_H
#define GAME_GPTCUBE_H

#include "glm/vec4.hpp"
#include "scene.h"
#include <iostream>

#define CUBE_SIZE 1.0
#define NUM_CUBES 3
#define DELTA  (NUM_CUBES-1)/2
#define  ROTATION_SPEED = 10.0f


class GPTCube {
public:
    GPTCube();
    void initCubePositions(Scene *scn);
    glm::vec4*  getCubeAngels();

    void  MoveUp();


    // 3D array for storing the positions and indices of the cube faces
    glm::vec4 cubePositions[NUM_CUBES][NUM_CUBES][NUM_CUBES];
    // 3D array for storing the angles and indices of the cube faces
    glm::vec4 cubeAngles[NUM_CUBES][NUM_CUBES][NUM_CUBES];



    std::vector<Shape *> *scn_shapes;
    glm::vec3 current_center;
    int clock_direction;
    int rotation_angle;

};


#endif //GAME_GPTCUBE_H
