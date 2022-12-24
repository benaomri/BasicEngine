#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include "Configuration/RubiksCube.h"

void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
			scn->Picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
        scn->MyTranslate(glm::vec3(0, 0, yoffset), 0);
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				    break;
				case GLFW_KEY_SPACE:
					scn->gptCube.MoveSPACE();
                    break;
                case GLFW_KEY_R:
                    scn->gptCube.MoveR();
                    break;
                case GLFW_KEY_L:
                    scn->gptCube.MoveL();
                    break;
                case GLFW_KEY_U:
                    scn->gptCube.MoveU();
                    break;
                case GLFW_KEY_D:
                    scn->gptCube.MoveD();
                    break;
                case GLFW_KEY_B:
                    scn->gptCube.MoveB();
                    break;
                case GLFW_KEY_F:
                    scn->gptCube.MoveF();
                    break;
                case GLFW_KEY_UP:
                    scn->gptCube.MoveUP();
                    break;
                case GLFW_KEY_DOWN:
                    scn->gptCube.MoveDOWN();
                    break;
                case GLFW_KEY_RIGHT:
                    scn->gptCube.MoveRIGHT();
                    break;
                case GLFW_KEY_LEFT:
                    scn->gptCube.MoveLEFT();
                    break;
                case GLFW_KEY_Z:
                    scn->gptCube.MoveZ();
                    break;
                case GLFW_KEY_A:
                    scn->gptCube.MoveA();
                    break;
			default:
				break;
			}
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->UpdatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->Resize(width,height);
		
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}
