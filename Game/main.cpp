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
//    unsigned char* data2 = scn->halfTone(image_data,width,height);
    scn->AddTexture(width, height, image_data);     // texture 1
    scn->AddTexture(width, height, image_data);     // texture 1
    scn->AddTexture(width, height, image_data);     // texture 2
    scn->AddTexture(width, height, image_data);     // texture 3

    scn->Init();

    display.SetScene(scn);


    // texture 0
    scn->SetShapeTex(0, 0);
    glViewport(0, 0, width, height);
    scn->Draw(1, 0, scn->BACK, true, false);

    // texture 1
    scn->SetShapeTex(0, 1);
    glViewport( DISPLAY_WIDTH / 2, 0, width, height);
    scn->Draw(1, 0, scn->BACK, false, false);

    // texture 2
    scn->SetShapeTex(0, 2);
    glViewport( DISPLAY_WIDTH / 2, DISPLAY_HEIGHT /2,width, height);
    scn->Draw(1, 0, scn->BACK, false, false);

    // texture 3
    scn->SetShapeTex(0, 3);
    glViewport(0, DISPLAY_HEIGHT /2, width, height);
    scn->Draw(1, 0, scn->BACK, false, false);

    display.SwapBuffers();
    while (!display.CloseWindow()){
        display.PollEvents();
    }

    delete scn;
    return 0;
}
