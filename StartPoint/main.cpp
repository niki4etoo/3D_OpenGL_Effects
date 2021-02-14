#include "include/glad/glad.h"

#ifndef _GLM_H_
#define _GLM_H_
#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#endif

#include <GLFW/glfw3.h>
#include <iostream>

#include "include/camera.h"
#include "include/shader.h"
#include "include/callbacks.h"
#include "include/texture_loader.h"
#include "include/input_processing.h"

#include "include/data_vertices.h"

const std::string title = "Start Point";

const std::string error_text_glfw_window = "Failed to create Window.";
const std::string error_text_glad_initialize = "Failed to initialize GLAD.";

float deltaTime = 0.0f, currentFrame = 0.0f, lastFrame = 0.0f;

Callbacks *callback = new Callbacks();
InputProcessing *input = new InputProcessing();

int main()
{
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
	
	//Mouse last position
	mouse.lastX = SCR_WIDTH / 2, mouse.lastY = SCR_HEIGHT / 2;
	
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title.c_str(), glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << error_text_glad_initialize << std::endl;
        return -1;
    }	
	
	glEnable(GL_DEPTH_TEST);
    // build and compile our shader program
    // ------------------------------------
    Shader ourShader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"); 
	
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
	
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dv.cube_vertices), dv.cube_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // texture1 attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	//Load Textures
	unsigned int texture1, texture2, texture3;
    TextureLoader tl1("textures/test1.jpg", texture1);
    TextureLoader tl2("textures/test2.jpg", texture2);
    TextureLoader tl3("textures/test3.jpg", texture3);

	ourShader.use();
	ourShader.setInt("texture_ID", 2);
	
	
	const float radius = 10.0f;
	float camX, camZ;
	
    // -----------
    while (!glfwWindowShouldClose(window))
    {
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
        // input
        // -----
        input->keyboard_input(window, deltaTime);
		std::cout << deltaTime << std::endl;
        // render
        // ------
        glClearColor(0.7f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		//Activate shader
        ourShader.use();
        
        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(mouse.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		for(unsigned int i = 0; i < 18; i++)
		{
			if(i % 3 == 0)
			{
				glBindTexture(GL_TEXTURE_2D, texture1);
			}
			else if (i % 2 == 0)
			{
				glBindTexture(GL_TEXTURE_2D, texture2);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, texture3);
			}
			
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, dv.cube_positions[i]);
			float angle = 40.0f;
			model = glm::rotate(model, glm::radians(angle) * ((float)glfwGetTime()*2), glm::vec3(0.5f, 0.5f, 0.5f));
			ourShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}
