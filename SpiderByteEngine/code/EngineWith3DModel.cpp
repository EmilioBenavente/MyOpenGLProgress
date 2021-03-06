//@NOTE(Emilio): Start making a head file to replace STD

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
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
void InitTexture(char* Path, int Num, int IsAlpha);

float DeltaTime = 0.0f;
float LastFrame = 0.0f;
float LastMouseXPos = 0.0f;
float LastMouseYPos = 0.0f;
int IsGameInit = 1;

FPSCamera MyCamera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 LightPos(1.2f, 5.0f, -6.0f);
//@NOTE(Emilio): Global Color Values
glm::vec3 SunLight(1.0f, 1.0f,1.0f);
glm::vec3 GreenLight(0.0f, 0.8f, 0.0f);
glm::vec3 CoralColor(1.0f, 0.5f, 0.31f);
glm::vec3 ResultColor = SunLight * CoralColor;


int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //@NOTE(Emilio): This is for MacOS Compilation
  //  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  GLFWwindow* MainWindow = glfwCreateWindow(WIDTH, HEIGHT,
					    "HelloOpenGL", 0,0);
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
  glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(MainWindow, MouseCallback);
  glfwSetScrollCallback(MainWindow, ScrollCallback);

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

  glm::vec3 LightPos[] =
    {
      glm::vec3(0.1f, 2.0f, -3.0f),
      glm::vec3(10.0f, 5.0f, 2.0f),
      glm::vec3(-2.0f, 3.0f, -2.0f),
      glm::vec3(5.0f, 5.0f, 4.0f),
    };

  Model ModelMesh("g:/SpiderByteEngine/res/Guitar/backpack.obj");

  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(LightCubeVerts),
	       LightCubeVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			3*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);
  

  Shader LightShader("g:/SpiderByteEngine/code/includes/Shader/DefaultLight.vs", "g:/SpiderByteEngine/code/includes/Shader/DefaultLight.fs");
  LightShader.Use();
  LightShader.SetVec3("LightColor", SunLight);


  Shader CubeShader("../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.vs", "../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.fs");
  CubeShader.Use();
  CubeShader.SetFloat("Offset", 0.26f);
  CubeShader.SetInt("UniTexCoords", 0);
  CubeShader.SetInt("UniTexCoords2", 1);
  CubeShader.SetFloat("Material.Shininess", 64.0f);  
  CubeShader.SetVec3("DirLight.Direction",glm::vec3(0.0f, -1.0f, 0.0f));
  CubeShader.SetVec3("ObjectColor", glm::vec3(0.1f));
  CubeShader.SetVec3("DirLight.AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("DirLight.DiffuseIntensity", glm::vec3(0.6));
  CubeShader.SetVec3("DirLight.SpecularIntensity", glm::vec3(0.7));  
  CubeShader.SetFloat("PointLights[0].ConstantK", 1.0f);  
  CubeShader.SetFloat("PointLights[0].LinearK", 0.09f);
  CubeShader.SetFloat("PointLights[0].QuadK", 0.32f);
  CubeShader.SetVec3("PointLights[0].AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("PointLights[0].DiffuseIntensity", glm::vec3(0.6));
  CubeShader.SetVec3("PointLights[0].SpecularIntensity",
		     glm::vec3(0.7));  
  CubeShader.SetFloat("PointLights[1].ConstantK", 1.0f);  
  CubeShader.SetFloat("PointLights[1].LinearK", 0.09f);
  CubeShader.SetFloat("PointLights[1].QuadK", 0.32f);
  CubeShader.SetVec3("PointLights[1].AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("PointLights[1].DiffuseIntensity", glm::vec3(0.6));
  CubeShader.SetVec3("PointLights[1].SpecularIntensity",
		     glm::vec3(0.7));  
  CubeShader.SetFloat("PointLights[2].ConstantK", 1.0f);  
  CubeShader.SetFloat("PointLights[2].LinearK", 0.09f);
  CubeShader.SetFloat("PointLights[2].QuadK", 0.32f);
  CubeShader.SetVec3("PointLights[2].AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("PointLights[2].DiffuseIntensity", glm::vec3(0.6));
  CubeShader.SetVec3("PointLights[2].SpecularIntensity",
		     glm::vec3(0.7));  
  CubeShader.SetFloat("PointLights[3].ConstantK", 1.0f);  
  CubeShader.SetFloat("PointLights[3].LinearK", 0.09f);
  CubeShader.SetFloat("PointLights[3].QuadK", 0.32f);
  CubeShader.SetVec3("PointLights[3].AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("PointLights[3].DiffuseIntensity", glm::vec3(0.6));
  CubeShader.SetVec3("PointLights[3].SpecularIntensity",
		     glm::vec3(0.7));  
  CubeShader.SetFloat("SpotLight.ConstantK", 1.0f);  
  CubeShader.SetFloat("SpotLight.LinearK", 0.09f);
  CubeShader.SetFloat("SpotLight.QuadK", 0.32f);
  CubeShader.SetVec3("SpotLight.AmbientIntensity", glm::vec3(0.2));
  CubeShader.SetVec3("SpotLight.DiffuseIntensity", glm::vec3(1.0));
  CubeShader.SetVec3("SpotLight.SpecularIntensity", glm::vec3(0.7));  
  CubeShader.SetFloat("SpotLight.SpotlightCutoffRAD",
		      glm::cos(glm::radians(15.0f)));
  CubeShader.SetFloat("SpotLight.OuterConeCutoffRAD",
		      glm::cos(glm::radians(32.0f)));


  
  glm::mat4 Persp = glm::perspective(glm::radians(45.0f),
				     (float)WIDTH / (float)HEIGHT,
				     0.1f, 100.0f);
  CubeShader.Use();  
  CubeShader.SetMat4("Projection", Persp);
  //@NOTE(Emilio): Main Game Loop
  while(!glfwWindowShouldClose(MainWindow))
    {      
      ProcessInput(MainWindow);

      float CurrentFrame = glfwGetTime();
      DeltaTime = CurrentFrame - LastFrame;
      LastFrame = CurrentFrame;

      float GreenValue = (sin(CurrentFrame) / 2.0f) + 0.5f;
      CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
			   0.35f, 1.0f);

      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glBindVertexArray(VAO);	      


      for(int i = 0; i < 4; ++i)
	{
	  glm::mat4 Model = glm::mat4(1.0f);
	  LightPos[i].z = GreenValue * 2.0f;
	  LightPos[i].y = GreenValue * 2.0f;
	  LightPos[i].x = GreenValue * 5.0f;
	  Model = glm::translate(Model, LightPos[i]);
	  LightShader.Use();
	  LightShader.SetMat4("View", MyCamera.GetViewMatrix());
	  Model = glm::rotate(Model,
			      glm::radians(GreenValue * 55.0f),
			      glm::vec3(6.14f, 6.36f, 6.52f));
	  Model = glm::translate(Model, LightPos[i]);	      
	  Model = glm::scale(Model, glm::vec3(0.2));
	  LightShader.SetMat4("Model", Model);
	  glBindVertexArray(VAO);	      
	  glDrawArrays(GL_TRIANGLES, 0, 36);
	}

      CubeShader.Use();
      glBindVertexArray(VAO);	      
      
      glm::mat4 Model = glm::mat4(1.0f);
      CubeShader.SetMat4("View", MyCamera.GetViewMatrix());
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPos[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPos[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPos[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPos[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
	      
      ModelMesh.Draw(CubeShader);

      
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

// void InitTexture(char* Path, int Num, int IsAlpha)
// {
//   unsigned int TextureID;
//   glGenTextures(1, &TextureID);
//   if(Num == 0)
//     {
//       glActiveTexture(GL_TEXTURE0);
//     }
//   else if(Num == 1)
//     {
//       glActiveTexture(GL_TEXTURE1);
//     }
//   else if(Num == 2)
//     {
//       glActiveTexture(GL_TEXTURE2);
//     }    
//   else if(Num == 3)
//     {
//       glActiveTexture(GL_TEXTURE3);
//     }
//   else
//     {
//       glActiveTexture(GL_TEXTURE4);
//     }    
  
//   glBindTexture(GL_TEXTURE_2D, TextureID);  
//   int ImgWidth;
//   int ImgHeight;
//   int ImgChannels;
//   unsigned char* data = stbi_load(Path, &ImgWidth, &ImgHeight,
// 				  &ImgChannels, 0);
//   if(data)
//     {
//       if(IsAlpha)
// 	{
// 	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImgWidth,
// 		       ImgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
// 		       data);
// 	}
//       else
// 	{
// 	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth,
// 		       ImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
// 		       data);
// 	}      
//       glGenerateMipmap(GL_TEXTURE_2D);
//     }
//   else
//     {
//       printf("SPI_ERROR -> Failed to load texture at path "
// 	     "%s\nWill render wierdly\n", Path);
//     }
//   stbi_image_free(data);
// }

