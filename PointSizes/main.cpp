//2D Translations, Rotations and Scaling

#include "include/glad/glad.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "include/camera.h"
#include "include/shader.h"
#include "include/callbacks.h"
#include "include/texture_loader.h"
#include "include/input_processing.h"

const std::string title = "Point Sizes";

const std::string error_text_glfw_window = "Failed to create Window.";
const std::string error_text_glad_initialize = "Failed to initialize GLAD.";

float deltaTime = 0.0f, currentFrame = 0.0f, lastFrame = 0.0f;

Callbacks* callback = new Callbacks();
InputProcessing* input = new InputProcessing();

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DECORATED, 0);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(primary);

	const unsigned int SCR_WIDTH = mode->width;
	const unsigned int SCR_HEIGHT = mode->height;

	// Mouse last position
	mouse.lastX = SCR_WIDTH / 2, mouse.lastY = SCR_HEIGHT / 2;

	// glfw window creation
	// --------------------
	GLFWwindow* window =
	    glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(),
			     glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		std::cout << error_text_glfw_window << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, callback->framebuffer_size);

	// Setting mouse input
	glfwSetCursorPosCallback(window, callback->mouse_input);
	glfwSetScrollCallback(window, callback->mouse_scroll);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << error_text_glad_initialize << std::endl;
		return -1;
	}

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	// build and compile our shader program
	// ------------------------------------
	Shader ourShader("shaders/vertex_shader.glsl",
			 "shaders/fragment_shader.glsl");

	float vertices[] = {
	    // first triangle
	    3.0f, 3.0f, 3.0f,    // top right
	    3.0f, -3.0f, 3.0f,   // bottom right
	    -3.0f, -3.0f, 3.0f,  // bottom left
	    // second triangle
	    1.5f, 1.5f, 3.5f,    // top right
	    -5.5f, -5.5f, 5.5f,  // bottom left
	    -8.5f, 8.5f, 8.5f,   // top left
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// bind the Vertex Array Object first, then bind and set vertex
	// buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void*)0);
	glEnableVertexAttribArray(0);

	const float radius = 10.0f;
	float camX, camZ;

	// -----------
	while (!glfwWindowShouldClose(window)) {
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		input->keyboard_input(window, deltaTime);

		// render
		// ------
		glClearColor(0.2f, 0.6f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate shader
		ourShader.use();

		// create transformations
		glm::mat4 view =
		    glm::mat4(1.0f);  // make sure to initialize matrix to
				      // identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);

		projection = glm::perspective(
		    glm::radians(mouse.fov),
		    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view =
		    glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// note: currently we set the projection matrix each frame, but
		// since the projection matrix rarely changes it's often best
		// practice to set it outside the main loop only once.
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		glBindVertexArray(VAO);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		ourShader.setMat4("model", model);
		glDrawArrays(GL_POINTS, 0, 6);

		// glfw: swap buffers and poll IO events (keys pressed/released,
		// mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their
	// purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
