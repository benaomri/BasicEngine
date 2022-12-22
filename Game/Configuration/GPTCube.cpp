
#include "GPTCube.h"

using namespace std;

GPTCube::GPTCube() {}

void GPTCube::initCubePositions(Scene *scn)
{
    scn_shapes = scn->getShapes();
    scn->AddTexture("../res/textures/plane.png", false);

    // initialize the positions and indices of the cube faces
    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int y = 0; y < NUM_CUBES; y++)
        {
            for (int z = 0; z < NUM_CUBES; z++)
            {
                float index =(float ) (x * NUM_CUBES * NUM_CUBES + y * NUM_CUBES + z);

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

                (*scn_shapes)[index]->MyTranslate(glm::vec3(x-DELTA, y-DELTA, z-DELTA), 0);
                (*scn_shapes)[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));

            }
        }
    }

    scn->MoveCamera(0, Scene::zTranslate, 15);
    current_center = glm::vec3(1, 1, 1);
    clock_direction = 1;
    rotation_angle = 90;

}

void  GPTCube::MoveUp() {
    cout<< "up"<< endl;
    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int y = 0; y < NUM_CUBES; y++)
        {
            for (int z = 0; z < NUM_CUBES; z++)
            {
                cubeAngles[x][y][z][1] += 10.0f;
            }
        }
    }
}