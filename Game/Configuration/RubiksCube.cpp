
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
// Determine the face of the Rubik's cube to be rotated
    int x = currentCenter.x + 1;

    if (x == NUM_CUBES)
    {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    // Rotate the cubes in the layer being rotated
    for (int y = 0; y < NUM_CUBES; y++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            int index = cubeAngles[x][y][z][3];

            // Call the three-argument version of the MyRotate function
            (*scnShapes)[index]->MyRotate(rotationAngle * clockDirection, glm::vec3(1, 0, 0), 0);
            // Update structure
            glm::vec4 newAngle = cubeAngles[x][y][z] + glm::vec4(rotationAngle * clockDirection, 0, 0, 0);
            cubeAngles[x][y][z] = angleModulo(newAngle);
        }
    }
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

void RubiksCube::MoveL()
{
    int x = currentCenter.x - 1;

    if (x == NUM_CUBES)
    {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    // Rotate the cubes in the layer being rotated
    for (int y = 0; y < NUM_CUBES; y++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            int index = cubeAngles[x][y][z][3];

            // Call the three-argument version of the MyRotate function
            (*scnShapes)[index]->MyRotate(rotationAngle * clockDirection, glm::vec3(1, 0, 0), 0);
            // Update structure
            glm::vec4 newAngle = cubeAngles[x][y][z] + glm::vec4(rotationAngle * clockDirection, 0, 0, 0);
            cubeAngles[x][y][z] = angleModulo(newAngle);
        }
    }
}


void RubiksCube::MoveU()
{
    // Determine the face of the Rubik's cube to be rotated
    int y = currentCenter.y + 1;

    if (y == NUM_CUBES)
    {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    // Rotate the cubes in the layer being rotated
    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            int index = cubeAngles[x][y][z][3];

            // Call the three-argument version of the MyRotate function
            (*scnShapes)[index]->MyRotate(rotationAngle * clockDirection, glm::vec3(0, 1, 0), 0);
            // Update structure
            glm::vec4 newAngle = cubeAngles[x][y][z] + glm::vec4(0, rotationAngle * clockDirection, 0, 0);
            cubeAngles[x][y][z] = angleModulo(newAngle);
        }
    }
}

void RubiksCube::MoveD() {}

void RubiksCube::MoveB() {}

void RubiksCube::MoveF() {}

void RubiksCube::MoveSPACE() {}

void RubiksCube::MoveZ() {}


void RubiksCube::MoveDOWN() {}

void RubiksCube::MoveLEFT() {}

void RubiksCube::MoveRIGHT() {}

void RubiksCube::MoveM() {}

void RubiksCube::updateStructure(glm::vec3 newPosition)
{
//    for (int i = 0; i < NUM_CUBES; i++)
//    {
//        for (int j = 0; j < NUM_CUBES; j++)
//        {
//            for (int k = 0; k < NUM_CUBES; k++)
//            {
//                int get_index = cubeAngles[i][j][k][3];
//
//                glm::mat4 get_rotation = (*scnShapes)[get_index]->GetRotate();
//                glm::mat3 cube_rotation = glm::mat3(
//                        glm::vec3(get_rotation[0].x, get_rotation[1].x, get_rotation[2].x),
//                        glm::vec3(get_rotation[0].y, get_rotation[1].y, get_rotation[2].y),
//                        glm::vec3(get_rotation[0].z, get_rotation[1].z, get_rotation[2].z)
//                );
//
//                glm::mat4 get_translation = (*scnShapes)[get_index]->GetTranslate();
//                glm::vec3 cube_position = glm::vec3(get_translation[3].x, get_translation[3].y, get_translation[3].z);
//
//                glm::vec3 new_cube_position = cube_position * cube_rotation;
//                glm::vec3 movement = new_cube_position - cube_position;
//                glm::vec3 final_movement = glm::vec3(round(movement.x), round(movement.y), round(movement.z));
//
//                cubeAngles[i + final_movement.x][j + final_movement.y][k + final_movement.z][3] = get_index;
//            }
//        }
//    }
}