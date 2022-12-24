
#include "RubiksCube.h"
using namespace std;

RubiksCube::RubiksCube() {}

void RubiksCube::initCubePositions(Scene *scn) {
    scn->AddTexture("../res/textures/plane.png", false);

    scnShapes = scn->getShapes();
    float distance = float(NUM_CUBES) - ((float(NUM_CUBES) + 1.f) * 0.5f);
    int index = 0;

    for (float i = -distance; i <= distance; i+=1)
    {
        std::vector<std::vector<std::pair<int, glm::vec3>>> sub_angles1;
        std::vector<std::vector<std::pair<int, glm::vec3>>> sub_centers1;

        for (float j = -distance; j <= distance; j+=1)
        {
            std::vector<std::pair<int, glm::vec3>> sub_angles2;
            std::vector<std::pair<int, glm::vec3>> sub_centers2;

            for (float k = -distance; k <= distance; k+=1)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*scnShapes)[index]->MyTranslate(glm::vec3(i, j, k), 0);
                (*scnShapes)[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));

                sub_angles2.push_back(std::make_pair(index, glm::vec3(0, 0, 0)));
                sub_centers2.push_back(std::make_pair(index, glm::vec3(i, j, k)));

                index++;
            }
            sub_angles1.push_back(sub_angles2);
            sub_centers1.push_back(sub_centers2);
        }
        cubeAngles.push_back(sub_angles1);
        cubePositions.push_back(sub_centers1);
    }
    scn->MoveCamera(0, Scene::zTranslate, 10);

    // Default global parameters
    currentCenter = glm::vec3(1, 1, 1);
    clockDirection = 1;
    rotationAngle = 90;

    // Animation parameters
    rotation_per_frame = 1;
    unlocked = true;
    activate_animation = true;
    animating = false;

    std::cout << "Cube Structure Completed" << std::endl;
}

void RubiksCube::MoveR() {

    int i = currentCenter.x + 1;

    if (i == NUM_CUBES) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int j = 0; j < NUM_CUBES; j++)
    {
        for (int k = 0; k < NUM_CUBES; k++)
        {
            glm::vec3 rotation_direction = glm::vec3(clockDirection, 0, 0);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();

}

void RubiksCube::MoveL()
{
    int i = currentCenter.x - 1;

    if (i == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int j = 0; j < NUM_CUBES; j++)
    {
        for (int k = 0; k < NUM_CUBES; k++)
        {
            glm::vec3 rotation_direction = glm::vec3(clockDirection, 0, 0);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();

}

void RubiksCube::MoveU()
{
    int j = currentCenter.y + 1;

    if (j == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < NUM_CUBES; i++)
    {
        for (int k = 0; k < NUM_CUBES; k++)
        {
            glm::vec3 rotation_direction = glm::vec3(0, clockDirection, 0);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();
}

void RubiksCube::MoveD()
{
    int j = currentCenter.y - 1;

    if (j == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < NUM_CUBES; i++)
    {
        for (int k = 0; k < NUM_CUBES; k++)
        {
            glm::vec3 rotation_direction = glm::vec3(0, clockDirection, 0);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();
}

void RubiksCube::MoveB()
{
    int k = currentCenter.z - 1;

    if (k == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < NUM_CUBES; i++)
    {
        for (int j = 0; j < NUM_CUBES; j++)
        {
            glm::vec3 rotation_direction = glm::vec3(0, 0, clockDirection);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();
}

void RubiksCube::MoveF()
{
    int k = currentCenter.z + 1;

    if (k == -1) {
        std::cout << "Invalid rotation" << std::endl;
        return;
    }

    for (int i = 0; i < NUM_CUBES; i++)
    {
        for (int j = 0; j < NUM_CUBES; j++)
        {
            glm::vec3 rotation_direction = glm::vec3(0, 0, clockDirection);
            Rotate_Cube(i, j, k, rotation_direction);
        }
    }
    updateStructure();
}

void RubiksCube::MoveSPACE() {clockDirection *= -1;}

void RubiksCube::MoveZ()
{
    if (rotationAngle > 90) {
        rotationAngle = rotationAngle / 2;
    }
    else {
        std::cout << "Rotation is already at minimum of 90" << std::endl;
    }
}

void RubiksCube::MoveA()
{
    if (rotationAngle < 180) {
        rotationAngle = rotationAngle * 2;
    }
    else {
        std::cout << "Rotation is already at maximum of 180" << std::endl;
    }
}

void RubiksCube::MoveUp()
{
    if (currentCenter.y < NUM_CUBES - 1) {
        currentCenter += glm::vec3(0, 1, 0);
    }
    else {
        std::cout << "Invalid center reposition" << std::endl;
    }
}

void RubiksCube::MoveDOWN()
{
    if (currentCenter.y > 0) {
        currentCenter -= glm::vec3(0, 1, 0);
    }
    else {
        std::cout << "Invalid center reposition" << std::endl;
    }
}

void RubiksCube::MoveLEFT()
{
    if (currentCenter.x > 0) {
        currentCenter -= glm::vec3(1, 0, 0);
    }
    else {
        std::cout << "Invalid center reposition" << std::endl;
    }
}

void RubiksCube::MoveRIGHT()
{
    if (currentCenter.x < NUM_CUBES - 1) {
        currentCenter += glm::vec3(1, 0, 0);
    }
    else {
        std::cout << "Invalid center reposition" << std::endl;
    }
}

void RubiksCube::MoveM() {}

glm::vec3 RubiksCube::angleModulo(glm::vec3 currentAngle) {
    if (currentAngle.x >= 360) {
        currentAngle = currentAngle - glm::vec3(360, 0, 0);
    }
    if (currentAngle.x < 0) {
        currentAngle = currentAngle + glm::vec3(360, 0, 0);
    }
    if (currentAngle.y >= 360) {
        currentAngle = currentAngle - glm::vec3(0, 360, 0);
    }
    if (currentAngle.y < 0) {
        currentAngle = currentAngle + glm::vec3(0, 360, 0);
    }
    if (currentAngle.z >= 360) {
        currentAngle = currentAngle - glm::vec3(0, 0, 360);
    }
    if (currentAngle.z < 0) {
        currentAngle = currentAngle + glm::vec3(0, 0, 360);
    }
    return currentAngle;
}

void RubiksCube::updateStructure()
{
    for (int i = 0; i < NUM_CUBES; i++)
    {
        for (int j = 0; j < NUM_CUBES; j++)
        {
            for (int k = 0; k < NUM_CUBES; k++)
            {
                int get_index = cubePositions[i][j][k].first;

                glm::mat4 get_rotation = (*scnShapes)[get_index]->GetRotate();
                glm::mat3 cube_rotation = glm::mat3(
                    glm::vec3(get_rotation[0].x, get_rotation[1].x, get_rotation[2].x),
                    glm::vec3(get_rotation[0].y, get_rotation[1].y, get_rotation[2].y),
                    glm::vec3(get_rotation[0].z, get_rotation[1].z, get_rotation[2].z)
                );

                glm::mat4 get_translation = (*scnShapes)[get_index]->GetTranslate();
                glm::vec3 cube_position = glm::vec3(get_translation[3].x, get_translation[3].y, get_translation[3].z);

                glm::vec3 new_cube_position = cube_position * cube_rotation;
                glm::vec3 movement = new_cube_position - cube_position;
                glm::vec3 final_movement = glm::vec3(round(movement.x), round(movement.y), round(movement.z));

                cubeAngles[i + final_movement.x][j + final_movement.y][k + final_movement.z].first = get_index;
            }
        }
    }
}

void RubiksCube::Rotate_Cube(int i, int j, int k, glm::vec3 rotation_direction) {
    int get_index = cubeAngles[i][j][k].first;

    // Getting the transposed rotation matrix
    glm::mat4 get_rotation = (*scnShapes)[get_index]->GetRotate();
    glm::mat3 cube_rotation = glm::mat3(
            glm::vec3(get_rotation[0].x, get_rotation[0].y, get_rotation[0].z),
            glm::vec3(get_rotation[1].x, get_rotation[1].y, get_rotation[1].z),
            glm::vec3(get_rotation[2].x, get_rotation[2].y, get_rotation[2].z)
    );

    // Rotating the cube
    glm::vec3 final_direction = rotation_direction * cube_rotation;
    (*scnShapes)[get_index]->MyRotate(rotationAngle, final_direction, 0);

    // Updating cubes angles structure
    glm::vec3 new_angle = glm::vec3(rotation_direction.x * rotationAngle, rotation_direction.y * rotationAngle, rotation_direction.z * rotationAngle) + cubeAngles[i][j][k].second;
    cubeAngles[i][j][k].second = angleModulo(new_angle);
}