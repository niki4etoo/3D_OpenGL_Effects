#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/shader.h"
#include "include/camera.h"
#include "include/model.h"
#include "include/mesh.h"
#include "include/callbacks.h"
#include "include/input_processing.h"
#include "include/loaders/texture_loader.h"

const std::string title = "Depth and Stencil Testing";

const std::string error_text_glfw_window = "Failed to create Window.";
const std::string error_text_glad_initialize = "Failed to initialize GLAD.";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Callbacks *callback = new Callbacks();
InputProcessing *input = new InputProcessing();

int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window =
	    glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << error_text_glfw_window << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, callback->framebuffer_size);
	glfwSetCursorPosCallback(window, callback->mouse_input);
	glfwSetScrollCallback(window, callback->mouse_scroll);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << error_text_glad_initialize << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);  // always pass the depth test (same effect as
			       // glDisable(GL_DEPTH_TEST))

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// build and compile shaders
	// -------------------------
	Shader shader("shaders/depth_testing_vertex.glsl",
		      "shaders/depth_testing_fragment.glsl");
	Shader shaderSingleColor("shaders/depth_testing_vertex.glsl",
				 "shaders/single_color_fragment.glsl");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float cubeVertices[] = {
	    // positions          // texture Coords
	    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
	    0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
	    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
	    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	    -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

	    -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

	    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
	    0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
	    0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
	    0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
	    -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	    -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
	    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	    -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

	float planeVertices[] = {
	    // positions          // texture Coords (note we set these higher
	    // than 1 (together with GL_REPEAT as texture wrapping mode). this
	    // will cause the floor texture to repeat)
	    15.0f, -0.501f, 15.0f,  5.0f,    0.0f,   -15.0f, -0.501f, 15.0f,
	    0.0f,  0.0f,    -15.0f, -0.501f, -15.0f, 0.0f,   5.0f,

	    15.0f, -0.501f, 15.0f,  5.0f,    0.0f,   -15.0f, -0.501f, -15.0f,
	    0.0f,  5.0f,    15.0f,  -0.501f, -15.0f, 5.0f,   5.0f};

	float wallsVertices[] = {
	    // positions				//texture coords
	    15.0f, -0.5f, 15.0f,  1.0f, 0.0f,  -15.0f, -0.5f, 15.0f,
	    0.0f,  0.0f,  -15.0f, 2.0f, 15.0f, 0.0f,   1.0f,

	    15.0f, -0.5f, 15.0f,  1.0f, 0.0f,  -15.0f, 2.0f,  15.0f,
	    0.0f,  1.0f,  15.0f,  2.0f, 15.0f, 1.0f,   1.0f,
	};

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices,
		     GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)(3 * sizeof(float)));
	glBindVertexArray(0);

	// plane VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices,
		     GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)(3 * sizeof(float)));
	glBindVertexArray(0);

	// Walls VAO
	unsigned int wallsVAO, wallsVBO;
	glGenVertexArrays(1, &wallsVAO);
	glGenBuffers(1, &wallsVBO);
	glBindVertexArray(wallsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, wallsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wallsVertices), &wallsVertices,
		     GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			      (void *)(3 * sizeof(float)));
	glBindVertexArray(0);

	// load textures
	// -------------
	unsigned int cubeTexture, floorTexture, wallsTexture;

	TextureLoader tl1("textures/floor.jpg", cubeTexture);
	TextureLoader tl2("textures/concrete.jpg", floorTexture);
	TextureLoader tl3("textures/walls.jpg", wallsTexture);

	// shader configuration
	// --------------------
	shader.use();
	shader.setInt("texture1", 0);

	float scale = 1.1f;
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		input->keyboard_input(window, deltaTime);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);

		// set uniforms
		shaderSingleColor.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(
		    glm::radians(mouse.fov),
		    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shaderSingleColor.setMat4("view", view);
		shaderSingleColor.setMat4("projection", projection);

		shader.use();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// draw floor as normal, but don't write the floor to the
		// stencil buffer, we only care about the containers. We set its
		// mask to 0x00 to not write to the stencil buffer.
		glStencilMask(0x00);
		// floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// walls
		glBindVertexArray(wallsVAO);
		glBindTexture(GL_TEXTURE_2D, wallsTexture);
		shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		// 1st. render pass, draw objects as normal, writing to the
		// stencil buffer
		// -----------------------------------------------------------------------
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		// cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// 2nd. render pass: now draw slightly scaled versions of the
		// objects, this time disabling stencil writing. Because the
		// stencil buffer is now filled with several 1s. The parts of
		// the buffer that are 1 are not drawn, thus only drawing the
		// objects' size differences, making it look like borders.
		// -----------------------------------------------------------------------------------------------------------------------------
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);
		shaderSingleColor.use();

		// cubes
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shaderSingleColor.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		// glfw: swap buffers and poll IO events (keys pressed/released,
		// mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their
	// purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}
