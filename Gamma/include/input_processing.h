#ifndef _INPUT_PROCESSING_H
#define _INPUT_PROCESSING_H

#include "camera.h"

class InputProcessing {
	bool gammaEnabled = false;
	bool gammaKeyPressed = false;
public:
	InputProcessing(){}
	~InputProcessing(){}
	bool getGammaEnabled()
	{
		return gammaEnabled;
	}
	void keyboard_input(GLFWwindow *window, float deltaTime) {		
		camera->MovementSpeed = 5.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
			
		//Camera controls
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->Position += camera->MovementSpeed * camera->Front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->Position -= camera->MovementSpeed * camera->Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->Position -= glm::normalize(glm::cross(camera->Front, camera->Up)) * camera->MovementSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->Position += glm::normalize(glm::cross(camera->Front, camera->Up)) * camera->MovementSpeed;
			
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !gammaKeyPressed)
		{
			gammaEnabled = !gammaEnabled;
			gammaKeyPressed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			gammaKeyPressed = false;
		}
	}
};

#endif
