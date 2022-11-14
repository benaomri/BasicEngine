#include "game.h"
#include "CannySobel.h"
#include "stb_image.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;
static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{
}

void Game::Init()
{
    int width, height, c;
	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
    unsigned char *image_data = stbi_load("../res/textures/lena256.jpg", &width, &height, &c, 4);
    vector<vector<unsigned char>> *asOneDemension= oneDemensionAndGray(width, height, image_data);
    AddTexture(width, height, FloydSteinbergAlgorithm(image_data));
//    unsigned char * ht = halfTone(image_data,width,height);
//    unsigned char *gray = toData(asOneDemension,image_data);
//    CannySobel *cannySobel = new CannySobel();
//    AddTexture(width, height, cannySobel->edgeDetector(width, height, asOneDemension));
	AddShape(Plane,-1,TRIANGLES);

	pickedShape = 0;

	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;

	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

unsigned char * Game::FloydSteinbergAlgorithm(unsigned char* data){
    int top = -256 * 4;
    int bottom	= 256 * 4;
    int right = 4;
    for(int x = 0; x < 256 * 255 * 4; x += 4) {
        double error = data[x] - ((data[x] / 16) * 16);
        //R
        data[x] = (data[x] / 16) * 16;
        data[x + bottom] = data[x + bottom] + (error * 7 / 16);
        data[x + right + top] = data[x + right + top] + (error * 3 / 16);
        data[x + right] = data[x + right] + (error * 5 / 16);
        data[x + right + bottom] = data[x + right + bottom] + (error * 1 / 16);
        //G
        data[x + 1] = data[x];
        data[x + 1 + bottom] = data[x + 1 + bottom] + (error * 7 / 16);
        data[x + 1 + right + top] = data[x + 1 + right + top] + (error * 3 / 16);
        data[x + 1 + right] = data[x + 1 + right] + (error * 5 / 16);
        data[x + 1 + right + bottom] = data[x + 1 + right + bottom] + (error * 1 / 16);
        //B
        data[x + 2] = data[x];
        data[x + 2 + bottom] = data[x + 2 + bottom] + (error * 7 / 16);
        data[x + 2 + right + top] = data[x + 2 + right + top] + (error * 3 / 16);
        data[x + 2 + right] = data[x + 2 + right] + (error * 5 / 16);
        data[x + 2 + right + bottom] = data[x + 2 + right + bottom] + (error * 1 / 16);
    }
    return data;
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}


vector<vector<unsigned char>>* Game::oneDemensionAndGray(int width, int height, unsigned char *image_data) {
    vector<vector<unsigned char>>* asOneDemension= new vector<vector<unsigned char >>();
    for(int x=0;x<width;x++) {
        vector<unsigned char> current_row;
        for (int y = 0; y < height; y++) {
            unsigned char avg = (image_data[4*(x*width+y)]+image_data[4*(x*width+y)+1]+image_data[4*(x*width+y)+2])/3;
            current_row.push_back(avg);
        }
        asOneDemension->push_back(current_row);
    }
    return asOneDemension;
}


unsigned char* Game::halfTone(unsigned char* image_data, int width, int height) {
    vector<vector<unsigned char>>* grey_scale_matrix = oneDemensionAndGray(width,height,image_data);
    vector<vector<unsigned char>>* newMat = new vector<vector<unsigned char>>();
    for (int i = 0; i < height; i++) {
        vector<unsigned char> inner1;
        vector<unsigned char> inner2;
        for (int j = 0; j < width; j++) {
            if ((*grey_scale_matrix)[i][j] <= 50) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(0);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 101) {
                inner1.push_back(0);
                inner1.push_back(0);
                inner2.push_back(255);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 152) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(0);
            }
            else if ((*grey_scale_matrix)[i][j] <= 203) {
                inner1.push_back(0);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            }
            else {
                inner1.push_back(255);
                inner1.push_back(255);
                inner2.push_back(255);
                inner2.push_back(255);
            }
        }
        newMat->push_back(inner1);
        newMat->push_back(inner2);
    }
    return toData(newMat,image_data);
}

Game::~Game(void)
{
}

unsigned char *Game::toData(std::vector<std::vector<unsigned char>> *oneDemensionAndGray, unsigned char *originData) {
    for(int i=0; i < oneDemensionAndGray->size(); i++){
        vector<unsigned char> current_row = oneDemensionAndGray->at(i);
        for(int j=0;j<current_row.size();j++)
        {
            originData[4*(i*current_row.size()+j)]=current_row.at(j);
            originData[4*(i*current_row.size()+j)+1]=current_row.at(j);
            originData[4*(i*current_row.size()+j)+2]=current_row.at(j);

        }
    }

    return originData;
}
