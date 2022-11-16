//#include "InputManager.h"
#include "display.h"
#include "game.h"
#include "glm/glm.hpp"
#include <stb_image.h>

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 512;
	const int DISPLAY_HEIGHT = 512;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;
    int width, height, c;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");


    unsigned char *image_data = stbi_load("../res/textures/lena256.jpg", &width, &height, &c, 4);
    unsigned char* halfToneData = scn->halfTone(image_data,width,height);
    unsigned char* cannySobelData=scn->findCannySobel(image_data,width,height);
    unsigned char *dataCopy = scn->copyData(image_data,width,height);
    unsigned char *FSdata= scn->FloydSteinbergAlgorithm(dataCopy);

    scn->AddTexture(2*width, 2*height, halfToneData);     //bottom left
    scn->AddTexture(width, height, FSdata);     // bottom-right
    scn->AddTexture(width, height, cannySobelData );     //  top-right
    scn->AddTexture(width, height, image_data);     // top left

    scn->Init();

    display.SetScene(scn);


    // bottom left
    scn->SetShapeTex(0, 0);
    glViewport(0, 0, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    scn->Draw(1, 0, scn->BACK, true, false);

    //bottom-right
    scn->SetShapeTex(0, 1);
    glViewport( DISPLAY_WIDTH / 2, 0, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    scn->Draw(1, 0, scn->BACK, false, false);

    //  top-right
    scn->SetShapeTex(0, 2);
    glViewport( DISPLAY_WIDTH / 2, DISPLAY_HEIGHT /2,DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    scn->Draw(1, 0, scn->BACK, false, false);

    //top left
    scn->SetShapeTex(0, 3);
    glViewport(0, DISPLAY_HEIGHT /2, DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2);
    scn->Draw(1, 0, scn->BACK, false, false);

    display.SwapBuffers();
    while (!display.CloseWindow()){
        display.PollEvents();
    }

    delete scn;
    return 0;
}
