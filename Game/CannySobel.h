#pragma once
#include "scene.h"

class CannySobel
{
public:

    unsigned char* edgeDetector(int width, int height, std::vector<std::vector<unsigned char>>* data);
    std::vector<std::vector<unsigned char>>* threshold(std::vector<std::vector<unsigned char>>* nms, int width, int height);
    void exportImage(const std::string& fileName, std::vector<std::vector<unsigned char>>* matrix, int width, int height, int base);
    std::vector<std::vector<unsigned char>>* hysteresis(std::vector<std::vector<unsigned char>>* image, int width, int height);
    bool inRange(int pos, int max);
    std::vector<std::vector<unsigned char>>* nonMaxSuppression(std::vector<std::vector<unsigned char>>* dx_plus_dy, int width, int height);
    void init(std::vector<std::vector<int>>* gauss, std::vector<std::vector<int>>* x, std::vector<std::vector<int>>* y,int *divG, int *divX, int *divY);
    std::vector<std::vector<unsigned char>>* matrixAddition(const std::vector<std::vector<char>>* dx, const std::vector<std::vector<char>>* dy, int width, int height);
    std::vector<std::vector<unsigned char>>* firstFilter(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div);
    std::vector<std::vector<char>>* secFilter(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div);




};