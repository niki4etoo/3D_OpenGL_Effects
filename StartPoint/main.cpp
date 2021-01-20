//2D Translations, Rotations and Scaling

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GLFW/glfw3.h>
#include <iostream>

#include "shader_s.h"


float cameraSpeed = 2.5f;
float deltaTime = 0.0f, currentFrame = 0.0f, lastFrame = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//Mouse parameters
bool firstMouse = true;
const float sensitivity = 0.1f;
float pitch = 0.0f, yaw = -90.0f;
float lastX = 0.0f, lastY = 0.0f;
float fov = 45.0f;

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
	lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
	
	
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Setting mouse input
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
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
    Shader ourShader("vertexShader.glsl", "fragmentShader.glsl"); 
    
    // set up vertex data ( Cube )
    // ------------------------------------------------------------------
    float vertices[] = {
		//front face
        //first triangle       	//Texture coords
         0.5f,  0.5f, 0.5f,	 	1.0f, 1.0f,		//top right
         0.5f, -0.5f, 0.5f,  	1.0f, 0.0f,		//bottom right
        -0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		//bottom left
        //second triangle
         0.5f,  0.5f, 0.5f,		1.0f, 1.0f,		//top right
        -0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		//bottom left
        -0.5f,  0.5f, 0.5f,		0.0f, 1.0f,		//top left
        
        //back face
        //first triangle		//Texture coords
        0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 	//top right
        0.5f, -0.5f, -0.5f, 	1.0f, 0.0f,		//bottom right
       -0.5f, -0.5f, -0.5f, 	0.0f, 0.0f, 	//bottom left 
       //second triangle
	    0.5f,  0.5f, -0.5f,		1.0f, 1.0f,		//top right
	   -0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		//bottom left
	   -0.5f,  0.5f, -0.5f,		0.0f, 1.0f,		//top left
	   
	   //top face
	   //first triangle
	    0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 	//top right
	    0.5f,  0.5f,  0.5f,		1.0f, 0.0f, 	//bottom right
	   -0.5f,  0.5f,  0.5f,		0.0f, 0.0f, 	//bottom left
	   //second triangle
	    0.5f,  0.5f, -0.5f,		1.0f, 1.0f, 	//top right
	   -0.5f,  0.5f,  0.5f, 	0.0f, 0.0f, 	//bottom left
	   -0.5f,  0.5f, -0.5f, 	0.0f, 1.0f, 	//top left
	   
	   //bottom face
	   //first triangle
	   0.5f, -0.5f, -0.5f, 		1.0f, 1.0f, 	//top right
	   0.5f, -0.5f,  0.5f,		1.0f, 0.0f, 	//bottom right
	  -0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 	//bottom left
	   //second triangle
	   0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 	//top right
	  -0.5f, -0.5f,  0.5f,		0.0f, 0.0f,		//bottom left
	  -0.5f, -0.5f, -0.5f, 		0.0f, 1.0f, 	//top left
	  
	  //left face
	  //first triangle
	  -0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		//top right
	  -0.5f, -0.5f,  0.5f, 		1.0f, 0.0f, 	//bottom right
	  -0.5f, -0.5f, -0.5f, 		0.0f, 0.0f, 	//bottom left
	  //second triangle
	  -0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		//top right
	  -0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 	//bottom left
	  -0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 	//top left
	  
	  //right face
	  //first triangle
	   0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		//top right
	   0.5f, -0.5f,  0.5f, 		1.0f, 0.0f, 	//bottom right
	   0.5f, -0.5f, -0.5f, 		0.0f, 0.0f, 	//bottom left
	  //second triangle
	   0.5f,  0.5f,  0.5f,		1.0f, 1.0f,		//top right
	   0.5f, -0.5f, -0.5f,		0.0f, 0.0f, 	//bottom left
	   0.5f,  0.5f, -0.5f,		0.0f, 1.0f, 	//top left
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

	//Load Textures
	unsigned int texture1, texture2, texture3;
    // Textures
    glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("test1.jpg", &width, &height, &nrChannels, 0);
    if(data)
    { 
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
	else
	{
		std::cout << "Error: cannot load file" << std::endl;
		return -1;
	}
	stbi_image_free(data);

	ourShader.use();
	ourShader.setInt("texture_ID", 0);
	
	// Textures
    glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	width = 0, height = 0, nrChannels = 0;
	unsigned char *data1 = stbi_load("test2.jpg", &width, &height, &nrChannels, 0);
    if(data1)
    { 
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    }
	else
	{
		std::cout << "Error: cannot load file" << std::endl;
		return -1;
	}
	stbi_image_free(data1);

	ourShader.use();
	ourShader.setInt("texture_ID", 1);
	
	// Textures
    glGenTextures(1, &texture3);
	glBindTexture(GL_TEXTURE_2D, texture3);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	width = 0, height = 0, nrChannels = 0;
	unsigned char *data2 = stbi_load("test3.jpg", &width, &height, &nrChannels, 0);
    if(data2)
    { 
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
    }
	else
	{
		std::cout << "Error: cannot load file" << std::endl;
		return -1;
	}
	stbi_image_free(data2);

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
        processInput(window, deltaTime);

        // render
        // ------
        glClearColor(0.7f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				
		//Activate shader
        ourShader.use();
        
        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		for(unsigned int i = 0; i < 12; i++)
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
			model = glm::translate(model, cubePositions[i]);
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, float deltaTime)
{
	cameraSpeed = 5.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
        
	//Camera controls
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	
	yaw += xoffset;
	pitch += yoffset;
	
	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
		
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

