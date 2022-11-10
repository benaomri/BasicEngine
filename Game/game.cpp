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
    int w, h, c;
	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");
    unsigned char *data = stbi_load("../res/textures/lena256.jpg", &w, &h, &c, 4);



    AddTexture(256,256,SobelOperator(w,h,data));

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

unsigned char* Game::SobelOperator(int width, int height, unsigned char* image)
{
    int pixel_x;
    int pixel_y;
    int sobel_x[3][3] =
            { { -1, 0, 1 },
              { -2, 0, 2 },
              { -1, 0, 1 } };

    int sobel_y[3][3] =
            { { -1, -2, -1 },
              { 0,  0,  0 },
              { 1,  2,  1 } };

    for (int x=1; x < (width-1)*4; x++)
    {
        for (int y=1; y < (height-1)*4; y+=3)
        {

            pixel_x = (sobel_x[0][0] * image[width * (y-1) + (x-1)])
                      + (sobel_x[0][1] * image[width * (y-1) +  x   ])
                      + (sobel_x[0][2] * image[width * (y-1) + (x+1)])
                      + (sobel_x[1][0] * image[width *  y    + (x-1)])
                      + (sobel_x[1][1] * image[width *  y    +  x   ])
                      + (sobel_x[1][2] * image[width *  y    + (x+1)])
                      + (sobel_x[2][0] * image[width * (y+1) + (x-1)])
                      + (sobel_x[2][1] * image[width * (y+1) +  x   ])
                      + (sobel_x[2][2] * image[width * (y+1) + (x+1)]);

            pixel_y = (sobel_y[0][0] * image[width * (y-1) + (x-1)])
                      + (sobel_y[0][1] * image[width * (y-1) +  x   ])
                      + (sobel_y[0][2] * image[width * (y-1) + (x+1)])
                      + (sobel_y[1][0] * image[width *  y    + (x-1)])
                      + (sobel_y[1][1] * image[width *  y    +  x   ])
                      + (sobel_y[1][2] * image[width *  y    + (x+1)])
                      + (sobel_y[2][0] * image[width * (y+1) + (x-1)])
                      + (sobel_y[2][1] * image[width * (y+1) +  x   ])
                      + (sobel_y[2][2] * image[width * (y+1) + (x+1)]);

            int val = (int)sqrt((pixel_x * pixel_x) + (pixel_y * pixel_y));

            if(val < 0) val = 0;
            if(val > 255) val = 255;

            image[height * y + x] = val;
            image[height * y + x+1] = val;
            image[height * y + x+2] = val;
        }
    }

    return image;
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
