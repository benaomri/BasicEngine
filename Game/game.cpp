#include "game.h"
#include "CannySobel.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>

using namespace std;

static void printMat(const glm::mat4 mat) {
    std::cout << " matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            std::cout << mat[j][i] << " ";
        std::cout << std::endl;
    }
}

Game::Game() : Scene() {
}

Game::Game(float angle, float relationWH, float near1, float far1) : Scene(angle, relationWH, near1, far1) {
}

void Game::Init() {
    AddShader("../res/shaders/pickingShader");
    AddShader("../res/shaders/basicShader");
    AddTexture("../res/textures/lena256.jpg", false);

    AddShape(Plane, -1, TRIANGLES);
    pickedShape = 0;
    SetShapeTex(0, 0);
    MoveCamera(0, zTranslate, 10);
    pickedShape = -1;

    //ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Model, const int shaderIndx) {
    Shader *s = shaders[shaderIndx];
    int r = ((pickedShape + 1) & 0x000000FF) >> 0;
    int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
    int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
    s->Bind();
    s->SetUniformMat4f("MVP", MVP);
    s->SetUniformMat4f("Normal", Model);
    s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
    if (shaderIndx == 0)
        s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
    else
        s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
    s->Unbind();
}

unsigned char *Game::FloydSteinbergAlgorithm(unsigned char *image_data) {
    int top = -256 * 4;
    int bottom = 256 * 4;
    int right = 4;
    for (int x = 0; x < 256 * 255 * 4; x += 4) {
        double error = image_data[x] - ((image_data[x] / 16) * 16);
        //R
        image_data[x] = (image_data[x] / 16) * 16;
        image_data[x + bottom] = image_data[x + bottom] + (error * 7 / 16);
        image_data[x + right + top] = image_data[x + right + top] + (error * 3 / 16);
        image_data[x + right] = image_data[x + right] + (error * 5 / 16);
        image_data[x + right + bottom] = image_data[x + right + bottom] + (error * 1 / 16);
        //G
        image_data[x + 1] = image_data[x];
        image_data[x + 1 + bottom] = image_data[x + 1 + bottom] + (error * 7 / 16);
        image_data[x + 1 + right + top] = image_data[x + 1 + right + top] + (error * 3 / 16);
        image_data[x + 1 + right] = image_data[x + 1 + right] + (error * 5 / 16);
        image_data[x + 1 + right + bottom] = image_data[x + 1 + right + bottom] + (error * 1 / 16);
        //B
        image_data[x + 2] = image_data[x];
        image_data[x + 2 + bottom] = image_data[x + 2 + bottom] + (error * 7 / 16);
        image_data[x + 2 + right + top] = image_data[x + 2 + right + top] + (error * 3 / 16);
        image_data[x + 2 + right] = image_data[x + 2 + right] + (error * 5 / 16);
        image_data[x + 2 + right + bottom] = image_data[x + 2 + right + bottom] + (error * 1 / 16);
    }
    exportImage("../img6.txt", oneDemensionAndGray(image_data,256,256), 256, 256, 16);
    return image_data;
}

void Game::WhenRotate() {
}

void Game::WhenTranslate() {
}

void Game::Motion() {
    if (isActive) {
    }
}


std::vector<std::vector<unsigned char>> *Game::oneDemensionAndGray(unsigned char *image_data, int width, int height) {
    vector<vector<unsigned char>> *asOneDemension = new vector<vector<unsigned char >>();
    for (int x = 0; x < width; x++) {
        vector<unsigned char> current_row;
        for (int y = 0; y < height; y++) {
            unsigned char avg = (image_data[4 * (x * width + y)] + image_data[4 * (x * width + y) + 1] +
                                 image_data[4 * (x * width + y) + 2]) / 3;
            current_row.push_back(avg);
        }
        asOneDemension->push_back(current_row);
    }
    return asOneDemension;
}


unsigned char *Game::halfTone(unsigned char *image_data, int width, int height) {
    vector<vector<unsigned char>> *grey_scale_matrix = oneDemensionAndGray(image_data, width, height);
    vector<vector<unsigned char>> *newMat = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> inner1;
        vector<unsigned char> inner2;
        for (int j = 0; j < width; j++) {
            if ((*grey_scale_matrix)[i][j] <= 50) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(0);
                inner2.push_back(0);
            } else if ((*grey_scale_matrix)[i][j] <= 101) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(255);
                inner2.push_back(0);
            } else if ((*grey_scale_matrix)[i][j] <= 152) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(0);
            } else if ((*grey_scale_matrix)[i][j] <= 203) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            } else {
                inner1.push_back(255);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            }
        }
        newMat->push_back(inner1);
        newMat->push_back(inner2);
    }
    width *= 2;
    height *= 2;
    exportImage("../img5.txt", newMat, width, height, 256);
    unsigned char* halfToneData = (unsigned char*)(malloc(4 * width * height));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            halfToneData[4 * (i * width + j)] = (*newMat)[i][j];
            halfToneData[4 * (i * width + j) + 1] = (*newMat)[i][j];
            halfToneData[4 * (i * width + j) + 2] = (*newMat)[i][j];
        }
    }
    return halfToneData;
}

Game::~Game(void) {
}

unsigned char *Game::copyData(unsigned char *originData,int width,int height) {
    vector<vector<unsigned char>> *oneDemensionAndGray= this->oneDemensionAndGray(originData, width, height);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * width * height));
    for (int i = 0; i < oneDemensionAndGray->size(); i++) {
        vector<unsigned char> current_row = oneDemensionAndGray->at(i);
        for (int j = 0; j < current_row.size(); j++) {
            data_copy[4 * (i * current_row.size() + j)] = current_row.at(j);
            data_copy[4 * (i * current_row.size() + j) + 1] = current_row.at(j);
            data_copy[4 * (i * current_row.size() + j) + 2] = current_row.at(j);

        }
    }

    return data_copy;
}

unsigned char *Game::findCannySobel(unsigned char *image_data, int width, int height) {
    CannySobel *cannySobel = new CannySobel();
    vector<vector<unsigned char>> *asOneDemension = oneDemensionAndGray(image_data, width, height);
    return cannySobel->edgeDetector(width, height, asOneDemension);

}

void Game::exportImage(const string& fileName, vector<vector<unsigned char>>* dataToExport, int width, int height, int base) {
    ofstream file;
    file.open(fileName);
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            file << ((int)(((*dataToExport)[x][y])) + 1) / base << ",";
        }
        file << endl;
    }
}