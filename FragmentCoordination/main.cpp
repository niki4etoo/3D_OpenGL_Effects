 #include "loaders/texture_loader.h"

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
	
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	
	const unsigned int SCR_WIDTH = 800;
	const unsigned int SCR_HEIGHT = 600;
	
	//Mouse last position
	lastX = SCR_WIDTH / 2, lastY = SCR_HEIGHT / 2;
	
	
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fragment Coordination variable", NULL, NULL);
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
        processInput(window, deltaTime);

        // render
        // ------
        glClearColor(0.7f, 0.4f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Activate shader
        lightingShader.use();
        
        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 150.0f);
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
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

