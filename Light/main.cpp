
#include "include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/shader.h"
#include "include/camera.h"
#include "include/callbacks.h"
#include "include/input_processing.h"
#include "include/loaders/texture_loader.h"

const std::string title = "Start Point";

const std::string error_text_glfw_window = "Failed to create Window.";
const std::string error_text_glad_initialize = "Failed to initialize GLAD.";

float deltaTime = 0.0f, currentFrame = 0.0f, lastFrame = 0.0f;

// Lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

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
	float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;

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

	glEnable(GL_DEPTH_TEST);
	// build and compile our shader program
	// ------------------------------------
	Shader lightingShader("shaders/vertex_shader.glsl",
			      "shaders/fragment_shader.glsl");

	// set up vertex data ( Cube )
	// ------------------------------------------------------------------
	float vertices[] = {
	    // front face
	    // first triangle       	//Normals				//Texture
	    // coords
	    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top right
	    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,    // top right
	    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
	    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // top left

	    // back face
	    // first triangle
	    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top right
	    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,    // top right
	    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom left
	    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,   // top left

	    // top face
	    // first triangle
	    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top right
	    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   // top right
	    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
	    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top left

	    // bottom face
	    // first triangle
	    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top right
	    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,   // top right
	    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
	    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top left

	    // left face
	    // first triangle
	    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top right
	    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top right
	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top left

	    // right face
	    // first triangle
	    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top right
	    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    // second triangle
	    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top right
	    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
	    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // top left
	};

	glm::vec3 cubePositions[] = {
	    glm::vec3(0.0f, 0.0f, -9.0f),   glm::vec3(2.0f, 5.0f, -30.0f),
	    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
	    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
	    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
	    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(4.5f, 10.2f, -4.5f),
	    glm::vec3(1.5f, 5.2f, -8.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

	glm::vec3 cubePositions2[] = {
	    glm::vec3(0.0f, 0.0f, 9.0f),   glm::vec3(-2.0f, -5.0f, 30.0f),
	    glm::vec3(1.5f, 2.2f, 2.5f),   glm::vec3(3.8f, 2.0f, 12.3f),
	    glm::vec3(-2.4f, 0.4f, 3.5f),  glm::vec3(1.7f, -3.0f, 7.5f),
	    glm::vec3(-1.3f, 2.0f, 2.5f),  glm::vec3(-1.5f, -2.0f, 2.5f),
	    glm::vec3(-1.5f, -0.2f, 1.5f), glm::vec3(-4.5f, -10.2f, 4.5f),
	    glm::vec3(-1.5f, -5.2f, 8.5f), glm::vec3(1.3f, -0.0f, 1.5f)};

	unsigned int lightVBO, lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			      (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int cubesVAO, cubesVBO;
	glGenVertexArrays(1, &cubesVAO);
	glGenBuffers(1, &cubesVBO);

	glBindVertexArray(cubesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
		     GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			      (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
			      (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Load Textures
	stbi_set_flip_vertically_on_load(true);
	unsigned int texture1, texture2, texture3;

	TextureLoader tl1("textures/test1.jpg", texture1);
	TextureLoader tl2("textures/test2.jpg", texture2);
	TextureLoader tl3("textures/test3.jpg", texture3);
	lightingShader.use();

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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Activate shader
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("lightPos", lightPos);

		// create transformations
		glm::mat4 view =
		    glm::mat4(1.0f);  // make sure to initialize matrix to
				      // identity matrix first
		glm::mat4 projection = glm::mat4(1.0f);

		projection = glm::perspective(
		    glm::radians(mouse.fov),
		    (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view =
		    glm::lookAt(camera->Position,
				camera->Position + camera->Front, camera->Up);

		/**
		 * 	Light objects
		 * */
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 12; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			if (i % 3 == 0)
				model = glm::scale(model, glm::vec3(0.2f));
			else if (i % 3 == 1)
				model = glm::scale(model, glm::vec3(0.4f));
			else
				model = glm::scale(model, glm::vec3(0.6f));

			float angle = 40.0f;
			model = glm::rotate(
			    model,
			    glm::radians(angle) * ((float)glfwGetTime() * 2),
			    glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setVec3("lightPos", lightPos);
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/**
		 * Cubes with textures in space
		 * */
		glBindVertexArray(cubesVAO);
		for (unsigned int i = 0; i < 12; i++) {
			glActiveTexture(GL_TEXTURE0);
			if (i % 3 == 0)
				glBindTexture(GL_TEXTURE_2D, texture3);
			else if (i % 3 == 1)
				glBindTexture(GL_TEXTURE_2D, texture2);
			else
				glBindTexture(GL_TEXTURE_2D, texture1);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions2[i]);
			float angle = 40.0f;
			model = glm::rotate(
			    model,
			    glm::radians(angle) * ((float)glfwGetTime() * 2),
			    glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released,
		// mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their
	// purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &cubesVAO);
	glDeleteBuffers(1, &lightVBO);
	glDeleteBuffers(1, &cubesVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}
