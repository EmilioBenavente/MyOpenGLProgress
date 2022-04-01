#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <math.h>

#include "includes/Shader/Shader.h"

#define WIDTH 800
#define HEIGHT 600

void FramebufferResize(GLFWwindow* Window, int Width, int Height);
void ProcessInput(GLFWwindow* Window);

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //@NOTE(Emilio): This is for MacOS Compilation
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* MainWindow = glfwCreateWindow(WIDTH, HEIGHT, "HelloOpenGL", 0,0);
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
  
  //@NOTE(Emilio): A collection of callback functions for opengl to use  
  glfwSetFramebufferSizeCallback(MainWindow, FramebufferResize);


  float TriVerts[] =
    {
      -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 
      0.0f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,
    };

  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(2, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(2, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TriVerts), TriVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);  

  
  Shader Shader("../SpiderByteEngine/code/includes/Shader/DefaultShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultShader.fs");
  Shader.Use();
  Shader.SetFloat("Offset", 0.26);
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //@NOTE(Emilio): Main Game Loop
  while(!glfwWindowShouldClose(MainWindow))
    {      
      ProcessInput(MainWindow);

      float TimeValue = glfwGetTime();
      float GreenValue = (sin(TimeValue) / 2.0f) + 0.5f;
      Shader.SetFloat4("UniColor", 0.1f, GreenValue, 0.35f, 1.0f);
      
      glClearColor(0.73f, 0.46f, 0.12f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glDrawArrays(GL_TRIANGLES, 0, 3);

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
}
