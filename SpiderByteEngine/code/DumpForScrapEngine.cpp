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
  
  glfwMakeContextCurrent(MainWindow);
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      printf("SPI_ERR -> Unable to load the GLAD library\n");
      glfwTerminate();
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(Quad),
		 Quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),(void*)0);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			  4 * sizeof(GL_FLOAT),
			  (void*)(2 *sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(2);


    glBindVertexArray(0);


    Shader QuadShader("../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.fs");
    QuadShader.Use();
    QuadShader.SetInt("FrameTex", 0);
    

    unsigned int FrameTex;
    glGenTextures(1, &FrameTex);
    glBindTexture(GL_TEXTURE_2D, FrameTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT,
		 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		    GL_LINEAR);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(LightCubeVerts),
		 LightCubeVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			  3*sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    Shader LightShader("../SpiderByteEngine/code/includes/Shader/DefaultLight.vs", "../SpiderByteEngine/code/includes/Shader/DefaultLight.fs");

    QuadShader.Use();
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
			   FrameTex, 0);
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			      GL_RENDERBUFFER, RBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      {
	printf("SPI_ERROR -> Unable to create Framebuffer"
	       "Address -> %d\nOr RenderBuffer of address"
	       "-> %d\n", FBO, RBO);
      }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while(!glfwWindowShouldClose(MainWindow))
      {
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	float CurrentFrame = glfwGetTime();
	float GreenValue = (sin(CurrentFrame) / 2.0f) + 0.5f;
	DeltaTime = CurrentFrame - LastFrame;
	LastFrame = CurrentFrame;

	ProcessInput(MainWindow);
	glm::mat4 Persp = glm::perspective(glm::radians(45.0f),
					   (float)WIDTH / (float)HEIGHT,
					   0.1f, 100.0f);
	glm::mat4 View = MyCamera.GetViewMatrix();
	glm::mat4 Model = glm::mat4(1.0f);

	LightShader.Use();
	LightShader.SetMat4("Projection", Persp);
	LightShader.SetMat4("View", View);
	LightShader.SetMat4("Model", Model);
	LightShader.SetVec3("LightColor", glm::vec3(0.45f, 0.2f, 0.7f));
	
	glBindVertexArray(LightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.7f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QuadShader.Use();
	glBindVertexArray(QuadVAO);
	glBindTexture(GL_TEXTURE_2D, FrameTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	
	glfwSwapBuffers(MainWindow);
	glfwPollEvents();
      }

    glfwTerminate();
    return 0;    
}


void FramebufferResize(GLFWwindow* Window, int Width, int Height)
{
  glViewport(0,0,Width,Height);
}

void ProcessInput(GLFWwindow* Window)
{
  if(glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(Window, GL_TRUE);
    }

  if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(FORWARD, DeltaTime);
    }
  if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(BACKWARD, DeltaTime);
    }
  if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(LEFT, DeltaTime);
    }
  if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(RIGHT, DeltaTime);
    }
  if(glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(DOWN, DeltaTime);
    }
  if(glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
    {
      MyCamera.ProcessKeyboard(UP, DeltaTime);
    }
}


void MouseCallback(GLFWwindow* Window, double XPos, double YPos)
{
  if(IsGameInit)
    {
      LastMouseXPos = XPos;
      LastMouseYPos = YPos;
      IsGameInit = 0;
    }
  float XOffset = XPos - LastMouseXPos;
  float YOffset = LastMouseYPos - YPos;
  LastMouseXPos = XPos;
  LastMouseYPos = YPos;

  MyCamera.ProcessMouseMovement(XOffset, YOffset);
}

void ScrollCallback(GLFWwindow* Window, double XOffset,
		    double YOffset)
{
  MyCamera.ProcessMouseScroll(YOffset);
}
