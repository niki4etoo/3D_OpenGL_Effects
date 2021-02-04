#ifndef _CALLBACKS_H
#define _CALLBACKS_H

#include "mouse.h"
#include "camera.h"

struct Mouse mouse;

Camera *camera = new Camera();

class Callbacks {

public:
	Callbacks(){};
	~Callbacks(){};
	
	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	// ---------------------------------------------------------------------------------------------
	static void framebuffer_size(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}
};
#endif
