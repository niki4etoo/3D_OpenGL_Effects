#ifndef INPUT_PROCESSING_H
#define INPUT_PROCESSING_H

#include "camera.h"

class InputProcessing {
       public:
	InputProcessing() {}
	~InputProcessing() {}
	void keyboard_input(GLFWwindow *window, float deltaTime) {
		camera->MovementSpeed = 5.5f * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		// Camera controls
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->Position +=
			    camera->MovementSpeed * camera->Front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->Position -=
			    camera->MovementSpeed * camera->Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->Position -= glm::normalize(glm::cross(
						camera->Front, camera->Up)) *
					    camera->MovementSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->Position += glm::normalize(glm::cross(
						camera->Front, camera->Up)) *
					    camera->MovementSpeed;
	}
};

#endif
