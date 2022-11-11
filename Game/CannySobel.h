//
// Created by noamm on 11/11/2022.
//
//
//#ifndef GAME_CANNYSOBEL_H
//#define GAME_CANNYSOBEL_H
//
//#endif //GAME_CANNYSOBEL_H


#pragma once
#include "scene.h"

class CannySobel
{
public:

    unsigned char* Canny_Edge_Detector(std::vector<std::vector<unsigned char>>* data, int width, int height);
    std::vector<std::vector<int>>* gaussianKernel(int* div);
    std::vector<std::vector<unsigned char>>* threshold(std::vector<std::vector<unsigned char>>* nms, int width, int height);
    void writeToFile(const std::string& fileName, std::vector<std::vector<unsigned char>>* mat, int width, int height, int div);
    std::vector<std::vector<unsigned char>>* hysteresis(std::vector<std::vector<unsigned char>>* threshold_image, int width, int height);
    bool inRange(int pos, int max);
    std::vector<std::vector<unsigned char>>* non_max_suppression(std::vector<std::vector<unsigned char>>* dx_plus_dy, int width, int height);
    std::vector<std::vector<int>>* dyKernel(int* div);
    std::vector<std::vector<int>>* dxKernel(int* div);
    std::vector<std::vector<unsigned char>>* matrixAddition(const std::vector<std::vector<char>>* dx, const std::vector<std::vector<char>>* dy, int width, int height);
    std::vector<std::vector<unsigned char>>* applyFilter1(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div);
    std::vector<std::vector<char>>* applyFilter2(std::vector<std::vector<unsigned char>>* mat, int width, int height, std::vector<std::vector<int>>* kernel, int div);




};