#include "CannySobel.h"
#include "stb_image.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<int>>* CannySobel::gaussianKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    kernel->push_back({ 1, 2, 1 });
    kernel->push_back({ 2, 4, 2 });
    kernel->push_back({ 1, 2, 1 });
    *div = 16;

    return kernel;
}

vector<vector<unsigned char>>* CannySobel::threshold(vector<vector<unsigned char>>* nms, int width, int height) {
    unsigned char low = 5;
    unsigned char high = 26;

    vector<vector<unsigned char>>* output = new vector<vector<unsigned char>>();

    for (int x = 0; x < height; x++) {
        vector<unsigned char> innerVec;
        for (int y = 0; y < width; y++) {
            if ((*nms)[x][y] >= high) {
                innerVec.push_back((unsigned char)255);
            }
            else if (low <= (*nms)[x][y] <= high) {
                innerVec.push_back((unsigned char)0);
            }
        }
        output->push_back(innerVec);
    }
    return output;
}

void CannySobel::exportImage(const string& fileName, vector<vector<unsigned char>>* matrix, int width, int height, int base) {
    ofstream file;
    file.open(fileName);
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            file << ((int)(((*matrix)[x][y])) + 1) / base << ",";
        }
        file << endl;
    }
}

vector<vector<unsigned char>>* CannySobel::hysteresis(vector<vector<unsigned char>>* image, int width, int height) {
    unsigned char edge = (unsigned char)255;

    vector<vector<unsigned char>>* Z = new vector<vector<unsigned char>>();

    for (int x = 0; x < height; x++) {
        vector<unsigned char> innerVec(width, 0);
        Z->push_back(innerVec);
    }

    for (int x = 1; x < height; x++) {
        for (int y = 1; y < width; y++) {
            if (image->at(x).at(y) == (unsigned char)255) {
                try {
                    if (image->at(x + 1).at(y - 1) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x + 1).at(y) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x + 1).at(y + 1) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x).at(y - 1) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x).at(y + 1) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x - 1).at(y - 1) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x - 1).at(y) == edge) {
                        Z->at(x).at(y) = edge;
                    }
                }
                catch (...) {
                    //cout << 'pass' << endl;
                }
                try {
                    if (image->at(x - 1).at(y + 1) == edge) {
                        Z->at(x).at(y) = edge;
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

vector<vector<unsigned char>>* CannySobel::nonMaxSuppression(vector<vector<unsigned char>>* dx_plus_dy, int width, int height) {
    vector<vector<unsigned char>>* output = new vector<vector<unsigned char>>();

    for (int i = 0; i < height; i++) {
        vector<unsigned char> innerVec(width, 0);
        output->push_back(innerVec);
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

    for (int x = 1; x < height; x++) {
        for (int y = 1; y < width; y++) {
            try {
                unsigned char q = 255;
                unsigned char r = 255;
                if ((0 <= (angle_matrix)[x][y] < 22.5) || (157.5 <= (angle_matrix)[x][y] <= 180)) {
                    if (inRange(x, height) && inRange(y + 1, width) && inRange(y - 1, width)) {
                        q = (*dx_plus_dy)[x][y + 1];
                        r = (*dx_plus_dy)[x][y - 1];
                    }
                }
                if (((*dx_plus_dy)[x][y] >= q) && ((*dx_plus_dy)[x][y] >= r)) {
                    (*output)[x][y] = (*dx_plus_dy)[x][y];
                }
                else {
                    (*output)[x][y] = (unsigned char)0;
                }
            }
            catch (int num) {
                cout << "err = " << num << endl;
            }
        }
    }
    return output;
}

vector<vector<int>>* CannySobel::dxKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    kernel->push_back({ 0, 0, 0 });
    kernel->push_back({ 0, -1, 0 });
    kernel->push_back({ 0, 1, 0 });
    *div = 1;
    return kernel;
}

vector<vector<int>>* CannySobel::dyKernel(int* div) {
    vector<vector<int>>* kernel = new vector<vector<int>>();
    kernel->push_back({ 0, 0, 0 });
    kernel->push_back({ 0, -1, 1 });
    kernel->push_back({ 0, 0, 0 });
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

vector<vector<unsigned char>>* CannySobel::firstFilter(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {

    vector<unsigned char> upper;
    for (int x = 0; x < width; x++) {
        upper.push_back(x % 100 + 100);
    }
    vector<vector<unsigned char>>* outputMatrix = new vector<vector<unsigned char>>();

    outputMatrix->push_back(upper);

    for (int y = 1; y < height - 1; y++) {
        vector<unsigned char> innerMat;
        innerMat.push_back(y % 100 + 100);
        for (int i = 1; i < width - 1; i++) {
            int temp = 0;
            temp += (*kernel)[0][0] * (*mat)[y - 1][i - 1];
            temp += (*kernel)[0][1] * (*mat)[y - 1][i];
            temp += (*kernel)[0][2] * (*mat)[y - 1][i + 1];
            temp += (*kernel)[1][0] * (*mat)[y][i - 1];
            temp += (*kernel)[1][1] * (*mat)[y][i];
            temp += (*kernel)[1][2] * (*mat)[y][i + 1];
            temp += (*kernel)[2][0] * (*mat)[y + 1][i - 1];
            temp += (*kernel)[2][1] * (*mat)[y + 1][i];
            temp += (*kernel)[2][2] * (*mat)[y + 1][i + 1];
            temp /= div;
            innerMat.push_back(abs(temp));
        }
        innerMat.push_back(y % 100 + 100);
        outputMatrix->push_back(innerMat);
    }
    vector<unsigned char> lower;
    for (int x = 0; x < width; x++) {
        lower.push_back(x % 100 + 100);
    }
    outputMatrix->push_back(lower);
    return outputMatrix;
}

vector<vector<char>>* CannySobel::secFilter(vector<vector<unsigned char>>* mat, int width, int height, vector<vector<int>>* kernel, int div) {
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

unsigned char* CannySobel::edgeDetector(int width, int height, vector<vector<unsigned char>>* data) {
    int div0;
    vector<vector<int>>* ones = gaussianKernel(&div0);
    vector<vector<unsigned char>>* afterFirstFilter = firstFilter(data, width, height, ones, div0);
    int divForX;
    vector<vector<int>>* kernelX = dxKernel(&divForX);
    int divFory;
    vector<vector<int>>* kernelY = dyKernel(&divFory);
    const vector<vector<char>>* dx = secFilter(afterFirstFilter, width, height, kernelX, divForX);
    const vector<vector<char>>* dy = secFilter(afterFirstFilter, width, height, kernelY, divFory);

    vector<vector<unsigned char>>* dx_plus_dy = matrixAddition(dx, dy, width, height);

    vector<vector<unsigned char>>* nonMax = nonMaxSuppression(dx_plus_dy, width, height);

    vector<vector<unsigned char>>* threshold_image = threshold(nonMax, width, height);

    vector<vector<unsigned char>>* hysteresis_image = hysteresis(threshold_image, width, height);

    exportImage("../img4.txt", hysteresis_image, width, height, 256);

    unsigned char* output = (unsigned char*)(malloc(4 * (width) * (height)));

    if (output != NULL) {
        for(int x = 0; x < height; x++) {
            for(int y = 0; y < width; y++) {
                output[4 * (x * width + y)] = (*hysteresis_image)[x][y];
                output[4 * (x * width + y) + sizeof(unsigned char)] = (*hysteresis_image)[x][y];
                output[4 * (x * width + y) + 2 * sizeof(unsigned char)] = (*hysteresis_image)[x][y];
            }
        }
    }

    return output;
}

