#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "includes/Textures/stb_image.h"
#include "includes/Shader/Shader.h"
#include "includes/Camera/FPSCamera.h"
#include "includes/Model/Model.h"

#define WIDTH 800
#define HEIGHT 600


void FramebufferResize(GLFWwindow* Window, int Width, int Height);
void MouseCallback(GLFWwindow* Window, double XPos, double YPos);
void ProcessInput(GLFWwindow* Window);
void ScrollCallback(GLFWwindow* Window, double XOffset,
		    double YOffset);
void InitTexture(unsigned int* TextureID, char* Path, int Num, int IsAlpha);


float DeltaTime = 0.0f;
float LastFrame = 0.0f;
float LastMouseXPos = 0.0f;
float LastMouseYPos = 0.0f;
int IsGameInit = 1;

FPSCamera MyCamera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,
		 GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* MainWindow = glfwCreateWindow(WIDTH,
					    HEIGHT,
					    "Scrap",
					    0,0);
  if(!MainWindow)
    {
      return -1;
    }
  glViewport(0,0,WIDTH, HEIGHT);

  
  //@NOTE(Emilio): A collection of callback functions for
  //opengl to us 
  glfwSetFramebufferSizeCallback(MainWindow,
				 FramebufferResize);
  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(MainWindow, GLFW_CURSOR,
		   GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(MainWindow, MouseCallback);
  glfwSetScrollCallback(MainWindow, ScrollCallback);
  
    float Quad[] =
    {
      -1.0f, -1.0f,  0.0f, 0.0f, 
       1.0f, -1.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  1.0f, 1.0f,       
    };  
    unsigned int QuadVAO;
    glGenVertexArrays(1, &QuadVAO);
    glBindVertexArray(QuadVAO);

    unsigned int QuadVBO;
    glGenBuffers(1, &QuadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVBO),
		 QuadVBO, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),
			  (void*)*(2 *sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);


    Shader QuadShader("../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.fs");
    QuadShader.Use();
    QuadShader.SetInt("FrameTex", 0);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		    GL_LINEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		    GL_LINEAR);
    unsigned int FrameTex;
    glGenTextures(1, &FrameTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, FrameTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT,
		 0, GL_RGB, GL_UNSIGNED_BYTE, 0);



    float LightCubeVerts[] =
      {
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f, 
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
      
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
      
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
      
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
      
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
      };

    unsigned int LightVAO;
    glGenVertexArrays(1,&LightVAO);
    glBindVertexArray(LightVAO);

    unsigned int LightVBO;
    glGenBuffers(1, &LightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, LightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(LightVBO),
		 LightVBO, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			  3*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexArribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			  3*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexArribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			  3*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexArribArray(2);

    
    glBindVertexArray(0);

    //@NOTE(Emilio): Double check but we should
    //be able to start making FBO and test GameLoop
    //underneath. Dont forget to add Camera Functions!!
    
}
