#include "RubiksCube.h"
using namespace std;

RubiksCube::RubiksCube() {}

void RubiksCube::initCubePositions(Scene *scn) {
    scn->AddTexture("../res/textures/plane.png", false);

    shapes = scn->getShapes();
    float distance = float(NUM_CUBES) - ((float(NUM_CUBES) + 1.f) * 0.5f);
    int index = 0;

    for (float x = -distance; x <= distance; x+=1)
    {
        vector<vector<pair<int, glm::vec3>>> angles;
        vector<vector<pair<int, glm::vec3>>> centers;

        for (float y = -distance; y <= distance; y+=1)
        {
            vector<pair<int, glm::vec3>> angles2;
            vector<pair<int, glm::vec3>> centers2;

            for (float z = -distance; z <= distance; z+=1)
            {
                scn->AddShape(Scene::Cube, -1, Scene::TRIANGLES);
                scn->SetShapeTex(index, 0);
                (*shapes)[index]->MyTranslate(glm::vec3(x, y, z), 0);
                (*shapes)[index]->MyScale(glm::vec3(0.5, 0.5, 0.5));

                angles2.push_back(make_pair(index, glm::vec3(0, 0, 0)));
                centers2.push_back(make_pair(index, glm::vec3(x, y, z)));

                index++;
            }
            angles.push_back(angles2);
            centers.push_back(centers2);
        }
        cubeAngles.push_back(angles);
        cubePositions.push_back(centers);
    }
    scn->MoveCamera(0, Scene::zTranslate, 10);

    currentCenter = glm::vec3(1, 1, 1);
    clockDirection = 1;
    rotationAngle = 90;


}

void RubiksCube::MoveR() {
    int x = currentCenter.x + 1;
    if (x == NUM_CUBES) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int y = 0; y < NUM_CUBES; y++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            glm::vec3 rotationDirection = glm::vec3(clockDirection, 0, 0);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();

}

void RubiksCube::MoveL()
{
    int x = currentCenter.x - 1;

    if (x == -1) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int y = 0; y < NUM_CUBES; y++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            glm::vec3 rotationDirection = glm::vec3(clockDirection, 0, 0);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();

}

void RubiksCube::MoveU()
{
    int y = currentCenter.y + 1;

    if (y == NUM_CUBES) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            glm::vec3 rotationDirection = glm::vec3(0, clockDirection, 0);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();
}

void RubiksCube::MoveD()
{
    int y = currentCenter.y - 1;

    if (y == -1) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int z = 0; z < NUM_CUBES; z++)
        {
            glm::vec3 rotationDirection = glm::vec3(0, clockDirection, 0);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();
}

void RubiksCube::MoveB()
{
    int z = currentCenter.z - 1;

    if (z == -1) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int y = 0; y < NUM_CUBES; y++)
        {
            glm::vec3 rotationDirection = glm::vec3(0, 0, clockDirection);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();
}

void RubiksCube::MoveF()
{
    int z = currentCenter.z + 1;

    if (z == NUM_CUBES) {
        cout << "ERROR: Invalid Rotation" << endl;
        return;
    }

    for (int x = 0; x < NUM_CUBES; x++)
    {
        for (int y = 0; y < NUM_CUBES; y++)
        {
            glm::vec3 rotationDirection = glm::vec3(0, 0, clockDirection);
            rotateCube(x, y, z, rotationDirection);
        }
    }
    updateStructure();
}

void RubiksCube::MoveSPACE()
{
    clockDirection *= -1;
}

void RubiksCube::MoveZ()
{
    if (rotationAngle > 90) {
        rotationAngle = rotationAngle / 2;
    }
    else {
        cout << "ERROR: Can't divide rotation.Rotation minimum is 90" << endl;
    }
}

void RubiksCube::MoveA()
{
    if (rotationAngle < 180) {
        rotationAngle = rotationAngle * 2;
    }
    else {
        cout << "ERROR: Can't multiply rotation. Rotation maximum is 180" << endl;
    }
}

void RubiksCube::MoveUP()
{
    if (currentCenter.y < NUM_CUBES - 1) {
        currentCenter += glm::vec3(0, 1, 0);
    }
    else {
        cout << "ERROR:Invalid center" << endl;
    }
}

void RubiksCube::MoveDOWN()
{
    if (currentCenter.y > 0) {
        currentCenter -= glm::vec3(0, 1, 0);
    }
    else {
        cout << "ERROR:Invalid center" << endl;
    }
}

void RubiksCube::MoveLEFT()
{
    if (currentCenter.x > 0) {
        currentCenter -= glm::vec3(1, 0, 0);
    }
    else {
        cout << "ERROR:Invalid center" << endl;
    }
}

void RubiksCube::MoveRIGHT()
{
    if (currentCenter.x < NUM_CUBES - 1) {
        currentCenter += glm::vec3(1, 0, 0);
    }
    else {
        cout << "ERROR:Invalid center" << endl;
    }
}


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

                glm::mat4 rotation = (*shapes)[get_index]->GetRotate();
                glm::mat3 cubeRotation = glm::mat3(
                    glm::vec3(rotation[0].x, rotation[1].x, rotation[2].x),
                    glm::vec3(rotation[0].y, rotation[1].y, rotation[2].y),
                    glm::vec3(rotation[0].z, rotation[1].z, rotation[2].z)
                );

                glm::mat4 translation = (*shapes)[get_index]->GetTranslate();
                glm::vec3 cubePosition = glm::vec3(translation[3].x, translation[3].y, translation[3].z);

                glm::vec3 newCubePosition = cubePosition * cubeRotation;
                glm::vec3 movement = newCubePosition - cubePosition;
                glm::vec3 finalMovement = glm::vec3(round(movement.x), round(movement.y), round(movement.z));

                cubeAngles[i + finalMovement.x][j + finalMovement.y][k + finalMovement.z].first = get_index;
            }
        }
    }
}

void RubiksCube::rotateCube(int x, int y, int z, glm::vec3 rotationDirection) {
    int index = cubeAngles[x][y][z].first;
    glm::mat4 rotation = (*shapes)[index]->GetRotate();
    glm::mat3 cubeRotation = glm::mat3(
            glm::vec3(rotation[0].x, rotation[0].y, rotation[0].z),
            glm::vec3(rotation[1].x, rotation[1].y, rotation[1].z),
            glm::vec3(rotation[2].x, rotation[2].y, rotation[2].z)
    );
    glm::vec3 finalDirection = rotationDirection * cubeRotation;
    (*shapes)[index]->MyRotate(rotationAngle, finalDirection, 0);

    glm::vec3 newAngle = glm::vec3(rotationDirection.x * rotationAngle, rotationDirection.y * rotationAngle, rotationDirection.z * rotationAngle) + cubeAngles[x][y][z].second;
    cubeAngles[x][y][z].second = angleModulo(newAngle);
}