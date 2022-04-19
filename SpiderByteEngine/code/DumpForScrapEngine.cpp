//@NOTE(Emilio): Start making a head file to replace STD

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <vector>
#include <math.h>

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
void InitCubeMap(unsigned int* CubeID, std::vector<char*> FaceTextures);


float GreenValue;

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
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //@NOTE(Emilio): This is for MacOS Compilation
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* MainWindow = glfwCreateWindow(WIDTH, HEIGHT,
					    "4D_CUBE", 0,0);
  if(!MainWindow)
    {
      printf("SPI_ERR -> Unable to create Window\n");
      glfwTerminate();
      return -1;
    }
  glfwMakeContextCurrent(MainWindow);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      printf("SPI_ERR -> Unable to load the GLAD library\n");
      glfwTerminate();
      return -1;
    }
  glViewport(0,0,WIDTH,HEIGHT);
  
  //@NOTE(Emilio): A collection of callback functions for
  //opengl to us 
  glfwSetFramebufferSizeCallback(MainWindow, FramebufferResize);
  glEnable(GL_DEPTH_TEST);
   // glEnable(GL_PROGRAM_POINT_SIZE);
  glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(MainWindow, MouseCallback);
  glfwSetScrollCallback(MainWindow, ScrollCallback);

  float Quad[] =
    {
      -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 
       1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  0.0f,  1.0f, 1.0f,       
    };
  float points[] = {
    -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
    0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
    -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
  };  

  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Quad),
	       Quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),
			(void*)0);
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),
			(void*)(3*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);
  

  //@NOTE(Emilio): Start of Uniform Buffer Objects
  unsigned int UBOMat;
  glGenBuffers(1, &UBOMat);
  glBindBuffer(GL_UNIFORM_BUFFER, UBOMat);
  glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), 0,
	       GL_STATIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBOMat, 0,2*sizeof(glm::mat4));

  glm::mat4 Persp = glm::perspective(glm::radians(45.0f),
				     (float)WIDTH / (float)HEIGHT,
				     0.1f, 100.0f);
  glBindBuffer(GL_UNIFORM_BUFFER, UBOMat);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
		  glm::value_ptr(Persp));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  
  //@NOTE(Emilio): Start of Shader Code
  Shader QuadShader("../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.fs", "../SpiderByteEngine/code/includes/Shader/DefaultShader.gs");
  QuadShader.Use();
   QuadShader.SetUniformBlockBinding("Matrices", 0);
  
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //@NOTE(Emilio): Main Game Loop
   while(!glfwWindowShouldClose(MainWindow))
    {
      float CurrentFrame = glfwGetTime();
      GreenValue = (sin(CurrentFrame) / 2.0f) + 0.5f;
      DeltaTime = CurrentFrame - LastFrame;
      LastFrame = CurrentFrame;

      ProcessInput(MainWindow);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      

      glm::mat4 View = MyCamera.GetViewMatrix();
      glBindBuffer(GL_UNIFORM_BUFFER, UBOMat);
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),
		      sizeof(glm::mat4), glm::value_ptr(View));
      glBindBuffer(GL_UNIFORM_BUFFER, 0);

      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::translate(Model, glm::vec3(1.0f));
      QuadShader.SetMat4("Model", Model);

      glBindVertexArray(VAO);	      	      
      glDrawArrays(GL_TRIANGLES, 0, 8);

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

