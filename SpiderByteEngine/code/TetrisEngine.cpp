#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "includes/Textures/stb_image.h"
#include "includes/Shader/Shader.h"
#include "includes/Camera/FPSCamera.h"

#define WIDTH 800
#define HEIGHT 600

void FramebufferResize(GLFWwindow* Window, int Width, int Height);
void MouseCallback(GLFWwindow* Window, double XPos, double YPos);
void ProcessInput(GLFWwindow* Window);
void ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset);
void InitTexture(char* Path, int Num, int IsAlpha);
void InitTexture(unsigned int* TextureID, char* Path, int Num, int IsAlpha);

float DeltaTime = 0.0f;
float LastFrame = 0.0f;
float LastMouseXPos = 0.0f;
float LastMouseYPos = 0.0f;

int IsGameInit = 1;

float MAX = 8.0f;

FPSCamera MyCamera(glm::vec3(0.0f, 0.0f, 15.0f));

glm::vec3 TPos[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(2.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 1.0f, 0.0f),    
};

glm::vec3 TRevPos[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(-1.0f, 1.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(1.0f, 1.0f, 0.0f),    
};

glm::vec3 Line[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(0.0f, 2.0f, 0.0f),
  glm::vec3(0.0f, 3.0f, 0.0f),    
};

glm::vec3 Square[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 1.0f, 0.0f),    
};

glm::vec3 SBlock[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(1.0f, 1.0f, 0.0f),
  glm::vec3(1.0f, 2.0f, 0.0f),    
};

glm::vec3 SRevBlock[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(0.0f, 1.0f, 0.0f),
  glm::vec3(-1.0f, 1.0f, 0.0f),
  glm::vec3(-1.0f, 2.0f, 0.0f),    
};

glm::vec3 LBlock[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 0.0f, 0.0f),
  glm::vec3(1.0f, 1.0f, 0.0f),
  glm::vec3(1.0f, 2.0f, 0.0f),    
};

glm::vec3 LRevBlock[] = {
  glm::vec3(0.0f, 0.0f, 0.0f),
  glm::vec3(-1.0f, 0.0f, 0.0f),
  glm::vec3(-1.0f, 1.0f, 0.0f),
  glm::vec3(-1.0f, 2.0f, 0.0f),    
};


struct Block
{
  glm::vec3 P1;
  glm::vec3 P2;
  glm::vec3 P3;
  glm::vec3 P4;  
};  

typedef struct Block Block;
Block Pieces[100];
int PieceIndex = 0;

int main()
{
  time_t t;
  srand((unsigned) time(&t));

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
  
  //@NOTE(Emilio): A collection of callback functions for opengl to us 
  glfwSetFramebufferSizeCallback(MainWindow, FramebufferResize);
  glEnable(GL_DEPTH_TEST);
  glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(MainWindow, MouseCallback);
  glfwSetScrollCallback(MainWindow, ScrollCallback);

  float TriVerts[] =
    {
      -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   0.4f, 0.0f,
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.6f, 0.0f,
      -0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.5f, 0.1f,

      -0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.5f, 0.1f,
      0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    0.6f, 0.0f,
      0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.4f, 0.0f,      
    };
  
  unsigned int VAO;
  unsigned int VBO;
  glGenVertexArrays(2, &VAO);
  glBindVertexArray(VAO);
  glGenBuffers(2, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TriVerts), TriVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);  
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(6*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(3);

  
  Shader Shader1 = Shader("../SpiderByteEngine/code/includes/Shader/DefaultShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultShader.fs");
  Shader1.Use();
  Shader1.SetFloat("Offset", 0.26f);
  Shader1.SetInt("UniTexCoords", 0);
  Shader1.SetInt("UniTexCoords2", 1);  
  glm::mat4 Persp = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
  Shader1.SetMat4("Projection", Persp);

  Shader CubeShader("../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.vs","../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.fs");  
  CubeShader.Use();
  CubeShader.SetFloat("Offset", 0.26f);
  CubeShader.SetInt("UniTexCoords", 0);
  CubeShader.SetInt("UniTexCoords2", 1);
  CubeShader.SetInt("Material.Ambient", 2);
  CubeShader.SetInt("Material.Diffuse", 3);
  CubeShader.SetInt("Material.Specular", 4);
  CubeShader.SetFloat("Material.Shininess", 64.0f);  
  CubeShader.SetVec3("DirLight.Direction",glm::vec3(0.0f, -1.0f, 0.0f));
  CubeShader.SetVec3("ObjectColor", glm::vec3(0.24, 0.01, 0.56));
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
		      glm::cos(glm::radians(1.0f)));
  CubeShader.SetFloat("SpotLight.OuterConeCutoffRAD",
		      glm::cos(glm::radians(20.0f)));

  CubeShader.SetMat4("Projection", Persp);
  
  unsigned int RegTex[5];
  InitTexture(&RegTex[0], "../SpiderByteEngine/res/container.jpg", 0, 0);
  InitTexture(&RegTex[1], "../SpiderByteEngine/res/awesomeface.png", 1, 1);
  InitTexture(&RegTex[2], "../SpiderByteEngine/res/container2_ambient.png", 2, 1);
  InitTexture(&RegTex[3], "../SpiderByteEngine/res/container2.png", 3, 1);  
  InitTexture(&RegTex[4], "../SpiderByteEngine/res/container2_specular.png", 4, 1);  
  CubeShader.SetMat4("View", MyCamera.GetViewMatrix());


  glm::vec3 LightPos[] =
    {
      glm::vec3(0.1f, 2.0f, -3.0f),
      glm::vec3(10.0f, 5.0f, 2.0f),
      glm::vec3(-2.0f, 3.0f, -2.0f),
      glm::vec3(5.0f, 5.0f, 4.0f),
    };
	  
  CubeShader.SetVec3("PointLights[0].Position", LightPos[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPos[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPos[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPos[3]);

  Block b;
  b.P1 = TPos[0];
  b.P2 = TPos[1];
  b.P3 = TPos[2];
  b.P4 = TPos[3];  
  Pieces[0] = b;

  CubeShader.Use();
  float MovementTime = 0.0f;
  //  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //@NOTE(Emilio): Main Game Loop
  while(!glfwWindowShouldClose(MainWindow))
    {      
      ProcessInput(MainWindow);

      float CurrentFrame = glfwGetTime();
      MovementTime += DeltaTime;
      DeltaTime = CurrentFrame - LastFrame;
      LastFrame = CurrentFrame;

      if(MovementTime > 2.0f)
	{
	  if(Pieces[PieceIndex].P1.y < -MAX)
	    {
	      Pieces[PieceIndex].P1.y = -MAX;
	    }
	  if(Pieces[PieceIndex].P1.y == -MAX)
	    {
	      PieceIndex++;
	      Block NewBlock;
	      int num = rand() % 8;
	      switch(num)
		{
		case 0:
		  {
		    NewBlock.P1 = TPos[0];
		    NewBlock.P2 = TPos[1];
		    NewBlock.P3 = TPos[2];
		    NewBlock.P4 = TPos[3];	      
		  }
		  break;
		case 1:
		  {
		    NewBlock.P1 = TRevPos[0];
		    NewBlock.P2 = TRevPos[1];
		    NewBlock.P3 = TRevPos[2];
		    NewBlock.P4 = TRevPos[3];	      
		  }
		  break;
		case 2:
		  {
		    NewBlock.P1 = Line[0];
		    NewBlock.P2 = Line[1];
		    NewBlock.P3 = Line[2];
		    NewBlock.P4 = Line[3];	      
		  }
		  break;
		case 3:
		  {
		    NewBlock.P1 = Square[0];
		    NewBlock.P2 = Square[1];
		    NewBlock.P3 = Square[2];
		    NewBlock.P4 = Square[3];	      
		  }
		  break;
		case 4:
		  {
		    NewBlock.P1 = SBlock[0];
		    NewBlock.P2 = SBlock[1];
		    NewBlock.P3 = SBlock[2];
		    NewBlock.P4 = SBlock[3];	      
		  }
		  break;
		case 5:
		  {
		    NewBlock.P1 = SRevBlock[0];
		    NewBlock.P2 = SRevBlock[1];
		    NewBlock.P3 = SRevBlock[2];
		    NewBlock.P4 = SRevBlock[3];	      
		  }
		  break;
		case 6:
		  {
		    NewBlock.P1 = LBlock[0];
		    NewBlock.P2 = LBlock[1];
		    NewBlock.P3 = LBlock[2];
		    NewBlock.P4 = LBlock[3];	      
		  }
		  break;
		default:
		  {
		    NewBlock.P1 = LRevBlock[0];
		    NewBlock.P2 = LRevBlock[1];
		    NewBlock.P3 = LRevBlock[2];
		    NewBlock.P4 = LRevBlock[3];	      
		  }
		}
	      Pieces[PieceIndex] = NewBlock;
	    }
	  else
	    {
	      Pieces[PieceIndex].P1.y -= 1;
	      Pieces[PieceIndex].P2.y -= 1;
	      Pieces[PieceIndex].P3.y -= 1;
	      Pieces[PieceIndex].P4.y -= 1;		  
	    }
	  MovementTime = 0.0f;
	}
      
      
      float GreenValue = (sin(CurrentFrame) / 2.0f) + 0.5f;
      float BackValue = (sin(CurrentFrame) / 9.0f) + 0.5f;
      CubeShader.SetFloat4("UniColor", 0.1f, GreenValue, 0.35f, 1.0f);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
      
      glClearColor(0.73f, BackValue, 1.0f - BackValue, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      MyCamera.Position = glm::vec3(0.0f, 0.0f, (GreenValue * 20.0f) + 3.0f);
      CubeShader.SetMat4("View", MyCamera.GetViewMatrix());
      for(int i = 0; i <= PieceIndex; ++i)
	{
	  glm::mat4 Model = glm::mat4(1.0f);
	  Model = glm::translate(Model, Pieces[i].P1);
	  CubeShader.SetMat4("Model", Model);
	  glDrawArrays(GL_TRIANGLES, 0, 6);

	  Model = glm::mat4(1.0f);
	  Model = glm::translate(Model, Pieces[i].P2);
	  CubeShader.SetMat4("Model", Model);
	  glDrawArrays(GL_TRIANGLES, 0, 6);

	  Model = glm::mat4(1.0f);
	  Model = glm::translate(Model, Pieces[i].P3);
	  CubeShader.SetMat4("Model", Model);
	  glDrawArrays(GL_TRIANGLES, 0, 6);

	  Model = glm::mat4(1.0f);
	  Model = glm::translate(Model, Pieces[i].P4);
	  CubeShader.SetMat4("Model", Model);
	  glDrawArrays(GL_TRIANGLES, 0, 6);
	  
	}
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
  if(glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
    {
      if(Pieces[PieceIndex].P1.y <= -MAX || Pieces[PieceIndex].P2.y <= -MAX ||
	 Pieces[PieceIndex].P3.y <= -MAX || Pieces[PieceIndex].P4.y <= -MAX )
	return;

      Pieces[PieceIndex].P1.y -= 1;
      Pieces[PieceIndex].P2.y -= 1;
      Pieces[PieceIndex].P3.y -= 1;
      Pieces[PieceIndex].P4.y -= 1;      
    }
  if(glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
    {
      if(Pieces[PieceIndex].P1.y >= MAX || Pieces[PieceIndex].P2.y >= MAX ||
	 Pieces[PieceIndex].P3.y >= MAX || Pieces[PieceIndex].P4.y >= MAX)
	return;

      Pieces[PieceIndex].P1.y += 1;
      Pieces[PieceIndex].P2.y += 1;
      Pieces[PieceIndex].P3.y += 1;
      Pieces[PieceIndex].P4.y += 1;      
    }
  if(glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
    {
      if(Pieces[PieceIndex].P1.x >= MAX || Pieces[PieceIndex].P2.x >= MAX ||
	 Pieces[PieceIndex].P3.x >= MAX || Pieces[PieceIndex].P4.x >= MAX )
	return;

      Pieces[PieceIndex].P1.x += 1;
      Pieces[PieceIndex].P2.x += 1;
      Pieces[PieceIndex].P3.x += 1;
      Pieces[PieceIndex].P4.x += 1;
    }
  if(glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
    {
      if(Pieces[PieceIndex].P1.x <= -MAX || Pieces[PieceIndex].P2.x <= -MAX ||
	 Pieces[PieceIndex].P3.x <= -MAX || Pieces[PieceIndex].P4.x <= -MAX )
	return;

      Pieces[PieceIndex].P1.x -= 1;
      Pieces[PieceIndex].P2.x -= 1;
      Pieces[PieceIndex].P3.x -= 1;
      Pieces[PieceIndex].P4.x -= 1;      
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

void ScrollCallback(GLFWwindow* Window, double XOffset, double YOffset)
{
  MyCamera.ProcessMouseScroll(YOffset);
}

void InitTexture(char* Path, int Num, int IsAlpha)
{
  unsigned int TextureID;
  glGenTextures(1, &TextureID);
  if(Num == 0)
    {
      glActiveTexture(GL_TEXTURE0);
    }
  else if(Num == 1)
    {
      glActiveTexture(GL_TEXTURE1);
    }
  else if(Num == 2)
    {
      glActiveTexture(GL_TEXTURE2);
    }    
  else if(Num == 3)
    {
      glActiveTexture(GL_TEXTURE3);
    }
  else
    {
      glActiveTexture(GL_TEXTURE4);
    }    

  glBindTexture(GL_TEXTURE_2D, TextureID);  
  int ImgWidth;
  int ImgHeight;
  int ImgChannels;
  unsigned char* data = stbi_load(Path, &ImgWidth, &ImgHeight, &ImgChannels, 0);
  if(data)
    {
      if(IsAlpha)
	{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImgWidth, ImgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
      else
	{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth, ImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}      
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
      printf("SPI_ERROR -> Failed to load texture at path %s\nWill render wierdly\n", Path);
    }
  stbi_image_free(data);
}

void InitTexture(unsigned int* TextureID, char* Path, int Num, int IsAlpha)
{
  glGenTextures(1, TextureID);  
  if(Num == 0)
    {
      glActiveTexture(GL_TEXTURE0);
    }
  else if(Num == 1)
    {
      glActiveTexture(GL_TEXTURE1);
    }
  else if(Num == 2)
    {
      glActiveTexture(GL_TEXTURE2);
    }    
  else if(Num == 3)
    {
      glActiveTexture(GL_TEXTURE3);
    }
  else
    {
      glActiveTexture(GL_TEXTURE4);
    }    

  glBindTexture(GL_TEXTURE_2D, *TextureID);


  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		  GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		  GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		  GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		  GL_LINEAR);

  if(!Path)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH,
		   HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
      return;
    }
  
  int ImgWidth;
  int ImgHeight;
  int ImgChannels;
  unsigned char* data = stbi_load(Path, &ImgWidth, &ImgHeight,
				  &ImgChannels, 0);
  if(data)
    {
      if(IsAlpha)
	{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ImgWidth,
		       ImgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		       data);
	}
      else
	{
	  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ImgWidth,
		       ImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		       data);
	}      
      glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
      printf("SPI_ERROR -> Failed to load texture at path "
	     "%s\nWill render wierdly\n", Path);
    }
  stbi_image_free(data);
}
