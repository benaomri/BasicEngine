#pragma once

#include "scene.h"

class Game : public Scene {
public:

    Game();

    Game(float angle, float relationWH, float near, float far);

    void Init();

    void Update(const glm::mat4 &MVP, const glm::mat4 &Model, const int shaderIndx);

    void ControlPointUpdate();

    void WhenRotate();

    void WhenTranslate();

    void Motion();

    ~Game(void);

    unsigned char* halfTone(unsigned char* image_data, int width, int height);
    unsigned char * copyData(unsigned char *originData,int width,int height);
    std::vector<std::vector<unsigned char>> *oneDemensionAndGray(unsigned char *image_data, int width, int height);
    unsigned char * FloydSteinbergAlgorithm(unsigned char* image_data);
    unsigned char * findCannySobel(unsigned char* image_data, int width, int height);
    void exportImage(const std::string& fileName, std::vector<std::vector<unsigned char>>* dataToExport, int width, int height, int base);

};

