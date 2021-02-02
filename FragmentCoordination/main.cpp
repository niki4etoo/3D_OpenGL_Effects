#include "include/glad/glad.h"
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/shader.h"
#include "include/camera.h"
#include "include/loaders/texture_loader.h"
#include "include/callbacks.h"
#include "include/input_processing.h"

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
	
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	
	//Mouse last position
	float lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
	
	
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fragment Coordination", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, callback->framebuffer_size);
	glfwSetCursorPosCallback(window, callback->mouse_input);
	glfwSetScrollCallback(window, callback->mouse_scroll);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }	
	
	glEnable(GL_DEPTH_TEST);
	
    // build and compile our shader program
    // ------------------------------------
    Shader lightingShader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"); 
    
    // set up vertex data ( Cube )
    // ------------------------------------------------------------------
    float vertices[] = {
		//front face
        //first triangle
         0.5f,  0.5f, 0.5f,	 	//top right
         0.5f, -0.5f, 0.5f,  	//bottom right
        -0.5f, -0.5f, 0.5f,		//bottom left
        //second triangle
         0.5f,  0.5f, 0.5f,		//top right
        -0.5f, -0.5f, 0.5f,		//bottom left
        -0.5f,  0.5f, 0.5f,		//top left
        
        //back face
        //first triangle		
        0.5f,  0.5f, -0.5f,		//top right
        0.5f, -0.5f, -0.5f, 	//bottom right
       -0.5f, -0.5f, -0.5f, 	//bottom left 
       //second triangle
	    0.5f,  0.5f, -0.5f,		//top right
	   -0.5f, -0.5f, -0.5f,		//bottom left
	   -0.5f,  0.5f, -0.5f,		//top left
	   
	   //top face
	   //first triangle
	    0.5f,  0.5f, -0.5f,		//top right
	    0.5f,  0.5f,  0.5f,		//bottom right
	   -0.5f,  0.5f,  0.5f,		//bottom left
	   //second triangle
	    0.5f,  0.5f, -0.5f,		//top right
	   -0.5f,  0.5f,  0.5f, 	//bottom left
	   -0.5f,  0.5f, -0.5f, 	//top left
	   
	   //bottom face
	   //first triangle
	   0.5f, -0.5f, -0.5f, 		//top right
	   0.5f, -0.5f,  0.5f,		//bottom right
	  -0.5f, -0.5f,  0.5f,		//bottom left
	   //second triangle
	   0.5f, -0.5f, -0.5f,		//top right
	  -0.5f, -0.5f,  0.5f,		//bottom left
	  -0.5f, -0.5f, -0.5f, 		//top left
	  
	  //left face
	  //first triangle
	  -0.5f,  0.5f,  0.5f,		//top right
	  -0.5f, -0.5f,  0.5f, 		//bottom right
	  -0.5f, -0.5f, -0.5f, 		//bottom left
	  //second triangle
	  -0.5f,  0.5f,  0.5f,		//top right
	  -0.5f, -0.5f, -0.5f,		//bottom left
	  -0.5f,  0.5f, -0.5f,		//top left
	  
	  //right face
	  //first triangle
	   0.5f,  0.5f,  0.5f,		//top right
	   0.5f, -0.5f,  0.5f, 		//bottom right
	   0.5f, -0.5f, -0.5f, 		//bottom left
	  //second triangle
	   0.5f,  0.5f,  0.5f,		//top right
	   0.5f, -0.5f, -0.5f,		//bottom left
	   0.5f,  0.5f, -0.5f,		//top left
    };
	
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  -9.0f), 
		glm::vec3( 2.0f,  5.0f, -30.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3( 4.5f,  10.2f, -4.5f), 
		glm::vec3( 1.5f,  5.2f, -8.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture1 attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


	lightingShader.use();

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

        // render
        // ------
        glClearColor(0.7f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Activate shader
        lightingShader.use();
        
        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(mouse.fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view = glm::lookAt(camera->Position, camera->Position + camera->Front, camera->Up);
        
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        
		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 12; i++)
		{				
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 40.0f;
			model = glm::rotate(model, glm::radians(angle) * ((float)glfwGetTime()*2), glm::vec3(0.5f, 0.5f, 0.5f));
			lightingShader.setMat4("model", model);
		
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
