#include "game.h"
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
    unsigned char *gray = toData(asOneDemension,image_data);
    AddTexture(256, 256,gray);
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


void Game::halfTone(int width,int height,unsigned char *image_data) {
//    double a=7/16;
//    double b=3/16;
//    double g=5/16;
//    double d = 1/16;
//    double e;
//
//    for (int i = 0; i < width*height*4; i=i+4) {
//            P(x,y) = trunc(I(x,y) + 0.5)
//            e = I(x,y) - P(x,y);
//            I(x,y+1) += a*e;
//            I(x+1,y-1) += b*e;
//            I(x+1,y) += g*e;
//            I(x+1,y+1) += d *e;
//        }
//    }

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
