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

    void grayScale(int width, int height, unsigned char *image_data);
    void halfTone(int width, int height, unsigned char *image_data);
    unsigned char * toData(std::vector<std::vector<unsigned char>> *oneDemension,unsigned char *originData);
    std::vector <std::vector<unsigned char>> *oneDemension(int width, int height, unsigned char *image_data);

};

