#include "CannySobel.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<int>>* CannySobel::gaussianKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 1, 2, 1 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 2, 4, 2 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 1, 2, 1 };
    kernel->push_back(vec3);
    *div = 16;

    return kernel;
}

vector<vector<unsigned char>>* CannySobel::threshold(vector<vector<unsigned char>>* nms, int width, int height) {
    unsigned char highThreshold = 26;
    unsigned char lowThreshold = 5;

    int M = height;
    int N = width;
    vector<vector<unsigned char>>* res = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < width; j++) {
            if ((*nms)[i][j] >= highThreshold) {
                innerVec.push_back((unsigned char)255);
            }
            else if (lowThreshold <= (*nms)[i][j] <= highThreshold) {
                innerVec.push_back((unsigned char)0);
            }
        }
        res->push_back(innerVec);
    }
    return res;
}

void CannySobel::writeToFile(const string& fileName, vector<vector<unsigned char>>* mat, int width, int height, int div) {
    // open a file in write mode
    ofstream outfile;
    outfile.open(fileName);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            outfile << ((int)(((*mat)[i][j])) + 1) / div << ",";
        }
        outfile << endl;
    }
}

vector<vector<unsigned char>>* CannySobel::hysteresis(vector<vector<unsigned char>>* threshold_image, int width, int height) {
    unsigned char strong = (unsigned char)255;

    int M = height;
    int N = width;
    vector<vector<unsigned char>>* Z = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec(width, 0);
        Z->push_back(innerVec);
    }

    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            if (threshold_image->at(i).at(j) == (unsigned char)255) {
                try {
                    if (threshold_image->at(i + 1).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i + 1).at(j) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i + 1).at(j + 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i).at(j + 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j - 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (threshold_image->at(i - 1).at(j + 1) == strong) {
                        Z->at(i).at(j) = strong;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
            }
        }
    }
    return Z;
}

bool CannySobel::inRange(int pos, int max) {
    return pos >= 0 && pos < max;
}

vector<vector<unsigned char>>* CannySobel::non_max_suppression(vector<vector<unsigned char>>* dx_plus_dy, int width, int height) {
    int M = height;
    int N = width;
    vector<vector<unsigned char>>* Z = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec(width, 0);
        Z->push_back(innerVec);
    }

    vector<vector<unsigned char>> angle_matrix;

    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < width; j++) {
            unsigned char angle = (*dx_plus_dy)[i][j] * (unsigned char)180 / 3.14;
            if (angle < 0) {
                angle += (unsigned char)180;
            }
            innerVec.push_back(angle);
        }
        angle_matrix.push_back(innerVec);
    }

    for (int i = 1; i < M; i++) {
        for (int j = 1; j < N; j++) {
            try {
                unsigned char q = 255;
                unsigned char r = 255;

                // angle 0
                if ((0 <= (angle_matrix)[i][j] < 22.5) || (157.5 <= (angle_matrix)[i][j] <= 180)) {
                    if (inRange(i, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy)[i][j + 1];
                        r = (*dx_plus_dy)[i][j - 1];
                    }
                }

                    // angle 45
                else if (22.5 <= (angle_matrix)[i][j] < 67.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy)[i + 1][j - 1];
                        r = (*dx_plus_dy)[i - 1][j + 1];
                    }
                }
                    // angle 90
                else if (67.5 <= (angle_matrix)[i][j] < 112.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j, N)) {
                        q = (*dx_plus_dy)[i + 1][j];
                        r = (*dx_plus_dy)[i - 1][j];
                    }
                }
                    // angle 135
                else if (112.5 <= (angle_matrix)[i][j] < 157.5) {
                    if (inRange(i + 1, M) && inRange(i - 1, M) && inRange(j + 1, N) && inRange(j - 1, N)) {
                        q = (*dx_plus_dy)[i - 1][j - 1];
                        r = (*dx_plus_dy)[i + 1][j + 1];
                    }
                }
                if (((*dx_plus_dy)[i][j] >= q) && ((*dx_plus_dy)[i][j] >= r)) {
                    (*Z)[i][j] = (*dx_plus_dy)[i][j];
                }
                else {
                    (*Z)[i][j] = (unsigned char)0;
                }
            }
            catch (int num) {
                cout << "err = " << num << endl;
            }
        }
    }
    return Z;
}

vector<vector<int>>* CannySobel::dxKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 0, 0, 0 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 0, -1, 0 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 0, 1, 0 };
    kernel->push_back(vec3);
    *div = 1;

    return kernel;
}

vector<vector<int>>* CannySobel::dyKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    vector<int> vec1 = { 0, 0, 0 };
    kernel->push_back(vec1);
    vector<int> vec2 = { 0, -1, 1 };
    kernel->push_back(vec2);
    vector<int> vec3 = { 0, 0, 0 };
    kernel->push_back(vec3);
    *div = 1;

    return kernel;
}

vector<vector<unsigned char>>* CannySobel::matrixAddition(const vector<vector<char>>* dx, const vector<vector<char>>* dy, int width, int height) {
    vector<vector<unsigned char>>* added = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec;
        for (int j = 0; j < width; j++) {
            innerVec.push_back(min(255, abs((*dx)[i][j]) + abs((*dy)[i][j])));
        }
        added->push_back(innerVec);
    }
    return added;
}

vector<vector<unsigned char>>* CannySobel::applyFilter1(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    unsigned char threshold = 35;
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();

    //upper end of the photo
    vector<unsigned char> upper;
    for (int i = 0; i < width; i++) {
        upper.push_back(i % 100 + 100);
    }
    newMat->push_back(upper);

    for (int j = 1; j < height - 1; j++) {
        vector<unsigned char> innerMat;
        innerMat.push_back(j % 100 + 100); // left most end of the photo
        for (int i = 1; i < width - 1; i++) {
            int temp = 0;
            temp += (*kernel)[0][0] * (*mat)[j - 1][i - 1];
            temp += (*kernel)[0][1] * (*mat)[j - 1][i];
            temp += (*kernel)[0][2] * (*mat)[j - 1][i + 1];
            temp += (*kernel)[1][0] * (*mat)[j][i - 1];
            temp += (*kernel)[1][1] * (*mat)[j][i];
            temp += (*kernel)[1][2] * (*mat)[j][i + 1];
            temp += (*kernel)[2][0] * (*mat)[j + 1][i - 1];
            temp += (*kernel)[2][1] * (*mat)[j + 1][i];
            temp += (*kernel)[2][2] * (*mat)[j + 1][i + 1];
            temp /= div;
            innerMat.push_back(abs(temp));
        }
        innerMat.push_back(j % 100 + 100); // right most end of the photo
        newMat->push_back(innerMat);
    }

    // lower end of the photo
    vector<unsigned char> lower;
    for (int i = 0; i < width; i++) {
        lower.push_back(i % 100 + 100);
    }
    newMat->push_back(lower);

    //print_picture(newMat, width, height);
    return newMat;
}

vector<vector<char>>* CannySobel::applyFilter2(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
    unsigned char threshold = 35;
    vector<vector<char>>* newMat = new vector<vector<char>>();

    //upper end of the photo
    vector<char> upper;
    for (int i = 0; i < width; i++) {
        upper.push_back(i % 100 + 100);
    }
    newMat->push_back(upper);

    for (int j = 1; j < height - 1; j++) {
        vector<char> innerMat;
        innerMat.push_back(j % 100 + 100); // left most end of the photo
        for (int i = 1; i < width - 1; i++) {
            int temp = 0;
            temp += (*kernel)[0][0] * (*mat)[j - 1][i - 1];
            temp += (*kernel)[0][1] * (*mat)[j - 1][i];
            temp += (*kernel)[0][2] * (*mat)[j - 1][i + 1];
            temp += (*kernel)[1][0] * (*mat)[j][i - 1];
            temp += (*kernel)[1][1] * (*mat)[j][i];
            temp += (*kernel)[1][2] * (*mat)[j][i + 1];
            temp += (*kernel)[2][0] * (*mat)[j + 1][i - 1];
            temp += (*kernel)[2][1] * (*mat)[j + 1][i];
            temp += (*kernel)[2][2] * (*mat)[j + 1][i + 1];
            temp /= div;
            innerMat.push_back(temp);
        }
        innerMat.push_back(j % 100 + 100); // right most end of the photo
        newMat->push_back(innerMat);
    }

    // lower end of the photo
    vector<char> lower;
    for (int i = 0; i < width; i++) {
        lower.push_back(i % 100 + 100);
    }
    newMat->push_back(lower);

    //print_picture(newMat, width, height);
    return newMat;
}

unsigned char* CannySobel::Canny_Edge_Detector(vector<vector<unsigned char>>* data, int width, int height) {
    // apply smoothing filter
    int div0;
    vector<vector<int>>* ones = gaussianKernel(&div0);
    vector<vector<unsigned char>>* smoothedPic = applyFilter1(data, width, height, ones, div0);

    // get derivative in x and y axis
    int div1;
    vector<vector<int>>* kernelX = dxKernel(&div1);
    int div2;
    vector<vector<int>>* kernelY = dyKernel(&div2);
    const vector<vector<char>>* dx = applyFilter2(smoothedPic, width, height, kernelX, div1);
    const vector<vector<char>>* dy = applyFilter2(smoothedPic, width, height, kernelY, div2);

    // combine the derivatives
    vector<vector<unsigned char>>* dx_plus_dy = matrixAddition(dx, dy, width, height);

    // non max suppression
    vector<vector<unsigned char>>* nms = non_max_suppression(dx_plus_dy, width, height);

    // threshold
    vector<vector<unsigned char>>* threshold_image = threshold(nms, width, height);

    // hysteresis
    vector<vector<unsigned char>>* hysteresis_image = hysteresis(threshold_image, width, height);

    // write to file
    writeToFile("../img4.txt", hysteresis_image, width, height, 256);

    unsigned char* data_copy = (unsigned char*)(malloc(4 * (width) * (height)));

    if (data_copy != NULL) {
        for(int i = 0; i < height; i++) {
            for(int j = 0; j < width; j++) {
                data_copy[4 * (i * width + j)] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * width + j) + sizeof(unsigned char)] = (*hysteresis_image)[i][j];
                data_copy[4 * (i * width + j) + 2 * sizeof(unsigned char)] = (*hysteresis_image)[i][j];
            }
        }
    }

    return data_copy;
}

