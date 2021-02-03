#ifndef _GLM_H_
#define _GLM_H_
#include "glm/glm.hpp"
#endif

struct data_vertices {
	float cube_vertices[180] = {
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
	
	glm::vec3 cube_positions[18] = {
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
		glm::vec3(-4.7f,  -3.0f, -17.5f),  
		glm::vec3( 6.3f, 2.0f, -10.5f),  
		glm::vec3( 8.5f,  2.0f, -20.5f), 
		glm::vec3( 5.5f,  -5.2f, -17.5f), 
		glm::vec3( 9.5f,  10.2f, -4.5f), 
		glm::vec3( 12.5f,  -5.2f, 8.5f), 
		glm::vec3(-18.3f,  10.0f, -10.5f)
	};
};

struct data_vertices dv;
