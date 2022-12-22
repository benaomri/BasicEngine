
#include "RubiksCube.h"

using namespace std;

RubiksCube::RubiksCube() {}

void RubiksCube::initCubePositions(Scene *scn) {
    scnShapes = scn->getShapes();
    scn->AddTexture("../res/textures/plane.png", false);

    // initialize the positions and indices of the cube faces
    for (int x = 0; x < NUM_CUBES; x++) {
        for (int y = 0; y < NUM_CUBES; y++) {
            for (int z = 0; z < NUM_CUBES; z++) {
                float index = (float) (x * NUM_CUBES * NUM_CUBES + y * NUM_CUBES + z);

                // calculate the position of the center of the cube face
                cubePositions[x][y][z] = glm::vec4(x * CUBE_SIZE - (NUM_CUBES - 1) * CUBE_SIZE / 2,
                                                   y * CUBE_SIZE - (NUM_CUBES - 1) * CUBE_SIZE / 2,
                                                   z * CUBE_SIZE - (NUM_CUBES - 1) * CUBE_SIZE / 2,
                                                   index);

                // initialize the angles and indices to zero
                cubeAngles[x][y][z] = glm::vec4(0.0, 0.0, 0.0, index);
                // create a new MovableGLM object
                // add a shape of a cube to the scene
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);

                (*scnShapes)[index]->MyTranslate(glm::vec3(x - DELTA, y - DELTA, z - DELTA), 0);
                (*scnShapes)[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));

            }
        }
    }

    scn->MoveCamera(0, Scene::zTranslate, 15);
    currentCenter = glm::vec3(1, 1, 1);
    clockDirection = 1;
    rotationAngle = 90;

}

void RubiksCube::MoveUp() {
    cout << "up" << endl;
//    for (int x = 0; x < NUM_CUBES; x++) {
//        for (int y = 0; y < NUM_CUBES; y++) {
//            for (int z = 0; z < NUM_CUBES; z++) {
//                int index = cubeAngles[x][y][z][3];
//                (*scnShapes)[index]->MyRotate(rotationAngle, glm::vec3(clockDirection, 0, 0), 0);
//            }
//        }
//    }
}


void RubiksCube::MoveR() {
    int x = currentCenter.x + 1;

    if (x == NUM_CUBES) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int y = 0; y < NUM_CUBES; y++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            int index = cubeAngles[x][y][z][3];

            (*scnShapes)[index]->MyRotate(rotationAngle, glm::vec3(clockDirection, 0, 0), 0);
            // Update structure
            glm::vec4 newAngle = cubeAngles[x][y][z] + glm::vec4 (rotationAngle * clockDirection,0,0,0);
            cubeAngles[x][y][z] = angleModulo(newAngle);
        }
    }
//    Update_Structure(x, 'i');
}

glm::vec4 RubiksCube::angleModulo(glm::vec4 currentAngle) {
    if (currentAngle.x >= 360) {
        currentAngle = currentAngle - glm::vec4(360, 0, 0,0);
    }
    if (currentAngle.x < 0) {
        currentAngle = currentAngle + glm::vec4(360, 0, 0,0);
    }
    if (currentAngle.y >= 360) {
        currentAngle = currentAngle - glm::vec4(0, 360, 0,0);
    }
    if (currentAngle.y < 0) {
        currentAngle = currentAngle + glm::vec4(0, 360, 0,0);
    }
    if (currentAngle.z >= 360) {
        currentAngle = currentAngle - glm::vec4(0, 0, 360,0);
    }
    if (currentAngle.z < 0) {
        currentAngle = currentAngle + glm::vec4(0, 0, 360,0);
    }
    return currentAngle;
}

void RubiksCube::MoveL() {}

void RubiksCube::MoveU() {}

void RubiksCube::MoveD() {}

void RubiksCube::MoveB() {}

void RubiksCube::MoveF() {}

void RubiksCube::MoveSPACE() {}

void RubiksCube::MoveZ() {}


void RubiksCube::MoveDOWN() {}

void RubiksCube::MoveLEFT() {}

void RubiksCube::MoveRIGHT() {}

void RubiksCube::MoveM() {}