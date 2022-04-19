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


//@NOTE(Emilio): Global VBOs/VAOs/FBOs/RBOs | AGAIN Just for this file
unsigned int VAO[3];
unsigned int VBO[3];

unsigned int WorldFrontVAO;
unsigned int WorldFrontVBO;

unsigned int WorldBackVAO;
unsigned int WorldBackVBO;

unsigned int WorldLeftVAO;
unsigned int WorldLeftVBO;

unsigned int WorldRightVAO;
unsigned int WorldRightVBO;

unsigned int WorldTopVAO;
unsigned int WorldTopVBO;

unsigned int WorldBottomVAO;
unsigned int WorldBottomVBO;

float GreenValue;
unsigned int CubeMap;
unsigned int RegTex[5];

unsigned int WorldFrontTexture;
unsigned int WorldBackTexture;
unsigned int WorldLeftTexture;
unsigned int WorldRightTexture;
unsigned int WorldTopTexture;
unsigned int WorldBottomTexture;

//@NOTE(Emilio): Global Light Positions | AGAIN Just for this file
glm::vec3 LightPosArray[] =
  {
    glm::vec3(0.1f, 2.0f, -3.0f),
    glm::vec3(10.0f, 5.0f, 2.0f),
    glm::vec3(-2.0f, 3.0f, -2.0f),
    glm::vec3(5.0f, 5.0f, 4.0f),
  };



//@NOTE(Emilio): Creating 6 Worlds then the outer world!
void RenderWorld1(Shader CubeShader, Shader LightShader,
		  Shader CubeMapShader);
void RenderWorld2(Shader CubeShader, Shader LightShader);
void RenderWorld3(Shader CubeShader, Shader LightShader);
void RenderWorld4(Shader CubeShader, Shader LightShader,
		  Shader CubeMapShader);
void RenderWorld5(Shader CubeShader, Shader LightShader);
void RenderWorld6(Shader CubeShader, Model Model);
void RenderMainWorld(Shader QuadShader, Shader CubeMapShader);

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


  float WorldFront[] =
    {
      -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 
       1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  0.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldFrontVAO);
  glGenBuffers(1, &WorldFrontVBO);
  glBindVertexArray(WorldFrontVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldFrontVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldFront),
	       WorldFront, GL_STATIC_DRAW);
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


  float WorldBack[] =
    {
       1.0f, -1.0f,  -5.0f,  0.0f, 0.0f, 
      -1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
       1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  -5.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldBackVAO);
  glGenBuffers(1, &WorldBackVBO);
  glBindVertexArray(WorldBackVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldBackVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldBack),
	       WorldBack, GL_STATIC_DRAW);
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

  float WorldLeft[] =
    {
      -1.0f, -1.0f,  -5.0f,  0.0f, 0.0f, 
      -1.0f, -1.0f,   0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,   0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,   0.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldLeftVAO);
  glGenBuffers(1, &WorldLeftVBO);
  glBindVertexArray(WorldLeftVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldLeftVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldLeft),
	       WorldLeft, GL_STATIC_DRAW);
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


  float WorldRight[] =
    {
       1.0f, -1.0f,   0.0f,  0.0f, 0.0f, 
       1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
       1.0f,  1.0f,   0.0f,  0.0f, 1.0f,
       1.0f,  1.0f,   0.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  -5.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldRightVAO);
  glGenBuffers(1, &WorldRightVBO);
  glBindVertexArray(WorldRightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldRightVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldRight),
	       WorldRight, GL_STATIC_DRAW);
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


  float WorldTop[] =
    {
      -1.0f,  1.0f,   0.0f,  0.0f, 0.0f, 
       1.0f,  1.0f,   0.0f,  1.0f, 0.0f,
      -1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
      -1.0f,  1.0f,  -5.0f,  0.0f, 1.0f,
       1.0f,  1.0f,   0.0f,  1.0f, 0.0f,
       1.0f,  1.0f,  -5.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldTopVAO);
  glGenBuffers(1, &WorldTopVBO);
  glBindVertexArray(WorldTopVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldTopVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldTop),
	       WorldTop, GL_STATIC_DRAW);
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


  float WorldBottom[] =
    {
      -1.0f, -1.0f,  -5.0f,  0.0f, 0.0f, 
       1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
      -1.0f, -1.0f,   0.0f,  0.0f, 1.0f,
      -1.0f, -1.0f,   0.0f,  0.0f, 1.0f,
       1.0f, -1.0f,  -5.0f,  1.0f, 0.0f,
       1.0f, -1.0f,   0.0f,  1.0f, 1.0f,       
    };  
  glGenVertexArrays(1, &WorldBottomVAO);
  glGenBuffers(1, &WorldBottomVBO);
  glBindVertexArray(WorldBottomVAO);
  glBindBuffer(GL_ARRAY_BUFFER, WorldBottomVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(WorldBottom),
	       WorldBottom, GL_STATIC_DRAW);
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

  
  
  float LightCubeVerts[] =
    {
      -1.0f,  1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f, -1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,

      -1.0f, -1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f, -1.0f,  1.0f,
      -1.0f, -1.0f,  1.0f,

      -1.0f,  1.0f, -1.0f,
      1.0f,  1.0f, -1.0f,
      1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f,  1.0f,
      -1.0f,  1.0f, -1.0f,

      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f,  1.0f,
      1.0f, -1.0f,  1.0f
    };

  float CubeVerts[] =
    {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
      
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
      
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
      
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
      
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
      -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
      -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

  
  glGenVertexArrays(3, &VAO[0]);
  glGenBuffers(3, &VBO[0]);
  glBindVertexArray(VAO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(LightCubeVerts),
	       LightCubeVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			3*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVerts),
	       CubeVerts, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			8*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			8*sizeof(GL_FLOAT),
			(void*)(3*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
			8*sizeof(GL_FLOAT),
			(void*)(2*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
			8*sizeof(GL_FLOAT),
			(void*)(6*sizeof(GL_FLOAT)));
  glEnableVertexAttribArray(3);
  

  glBindVertexArray(VAO[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Quad),
	       Quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),(void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
			5*sizeof(GL_FLOAT),(void*)(3 * sizeof(GL_FLOAT)));
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
  Shader LightShader("../SpiderByteEngine/code/includes/Shader/DefaultLight.vs", "../SpiderByteEngine/code/includes/Shader/DefaultLight.fs",0);
  LightShader.Use();
  LightShader.SetVec3("LightColor", SunLight);

  Shader QuadShader("../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.vs", "../SpiderByteEngine/code/includes/Shader/DefaultFrameShader.fs", 0);
  QuadShader.Use();
  QuadShader.SetInt("FrameTex", 0);

  Shader CubeMapShader("../SpiderByteEngine/code/includes/Shader/DefaultCube.vs","../SpiderByteEngine/code/includes/Shader/DefaultCube.fs", 0);
  CubeMapShader.Use();
  CubeMapShader.SetVec4("TextureMult", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
  
  Shader CubeShader("../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.vs","../SpiderByteEngine/code/includes/Shader/DefaultShaderWithTextureMapping.fs", "../SpiderByteEngine/code/includes/Shader/DefaultShader.gs");  
  CubeShader.Use();
  CubeShader.SetUniformBlockBinding("Matrices", 0);
  CubeShader.SetFloat("Offset", 0.26f);
  CubeShader.SetInt("UniTexCoords", 0);
  CubeShader.SetInt("UniTexCoords2", 1);
  CubeShader.SetInt("Material.Ambient", 2);
  CubeShader.SetInt("Material.Diffuse", 3);
  CubeShader.SetInt("Material.Specular", 4);
  CubeShader.SetFloat("Material.Shininess", 64.0f);  
  CubeShader.SetVec3("DirLight.Direction",glm::vec3(0.0f, -1.0f, 0.0f));
  CubeShader.SetVec3("ObjectColor", CoralColor);
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
  

  InitTexture(&RegTex[0], "../SpiderByteEngine/res/container.jpg", 0, 0);
  InitTexture(&RegTex[1], "../SpiderByteEngine/res/awesomeface.png", 1, 1);
  InitTexture(&RegTex[2], "../SpiderByteEngine/res/container2_ambient.png", 2, 1);
  InitTexture(&RegTex[3], "../SpiderByteEngine/res/container2.png", 3, 1);  
  InitTexture(&RegTex[4], "../SpiderByteEngine/res/container2_specular.png", 4, 1);  

  
  LightShader.Use();
  LightShader.SetMat4("Projection", Persp);
  QuadShader.Use();
  QuadShader.SetMat4("Projection", Persp);
  CubeMapShader.Use();
  CubeMapShader.SetMat4("Projection", Persp);
  
  
  unsigned int FBOFront;
  glGenFramebuffers(1, &FBOFront);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOFront);
  InitTexture(&WorldFrontTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldFrontTexture, 0);
  unsigned int RBOFront;
  glGenRenderbuffers(1, &RBOFront);
  glBindRenderbuffer(GL_RENDERBUFFER, RBOFront);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBOFront);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBOFront, RBOFront);
    }

  
  unsigned int FBOBack;
  glGenFramebuffers(1, &FBOBack);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOBack);
  InitTexture(&WorldBackTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldBackTexture, 0);
  unsigned int RBOBack;
  glGenRenderbuffers(1, &RBOBack);
  glBindRenderbuffer(GL_RENDERBUFFER, RBOBack);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBOBack);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBOBack, RBOBack);
    }


  unsigned int FBOLeft;
  glGenFramebuffers(1, &FBOLeft);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOLeft);
  InitTexture(&WorldLeftTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldLeftTexture, 0);
  unsigned int RBOLeft;
  glGenRenderbuffers(1, &RBOLeft);
  glBindRenderbuffer(GL_RENDERBUFFER, RBOLeft);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBOLeft);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBOLeft, RBOLeft);
    }

  unsigned int FBORight;
  glGenFramebuffers(1, &FBORight);
  glBindFramebuffer(GL_FRAMEBUFFER, FBORight);
  InitTexture(&WorldRightTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldRightTexture, 0);
  unsigned int RBORight;
  glGenRenderbuffers(1, &RBORight);
  glBindRenderbuffer(GL_RENDERBUFFER, RBORight);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBORight);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBORight, RBORight);
    }


  unsigned int FBOTop;
  glGenFramebuffers(1, &FBOTop);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOTop);
  InitTexture(&WorldTopTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldTopTexture, 0);
  unsigned int RBOTop;
  glGenRenderbuffers(1, &RBOTop);
  glBindRenderbuffer(GL_RENDERBUFFER, RBOTop);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBOTop);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBOTop, RBOTop);
    }

  unsigned int FBOBottom;
  glGenFramebuffers(1, &FBOBottom);
  glBindFramebuffer(GL_FRAMEBUFFER, FBOBottom);
  InitTexture(&WorldBottomTexture, 0,0,0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			 GL_TEXTURE_2D, WorldBottomTexture, 0);
   glBindFramebuffer(GL_FRAMEBUFFER, 0);
  unsigned int RBOBottom;
  glGenRenderbuffers(1, &RBOBottom);
  glBindRenderbuffer(GL_RENDERBUFFER, RBOBottom);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
			WIDTH, HEIGHT);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,
			    GL_DEPTH_STENCIL_ATTACHMENT,
			    GL_RENDERBUFFER, RBOBottom);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
     GL_FRAMEBUFFER_COMPLETE)
    {
      printf("SPI_ERROR -> Unable to create Framebuffer"
	     "Address -> %d\nOr RenderBuffer of address"
	     "-> %d\n", FBOBottom, RBOBottom);
    }
  

   std::vector<char*> Faces
     {
       "g:/SpiderByteEngine/res/skybox/right.jpg",
       "g:/SpiderByteEngine/res/skybox/left.jpg",
       "g:/SpiderByteEngine/res/skybox/top.jpg",
       "g:/SpiderByteEngine/res/skybox/bottom.jpg",
       "g:/SpiderByteEngine/res/skybox/front.jpg",
       "g:/SpiderByteEngine/res/skybox/back.jpg"              
     };
   InitCubeMap(&CubeMap, Faces);

   Model ModelMesh("g:/SpiderByteEngine/res/Guitar/backpack.obj");
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //@NOTE(Emilio): Main Game Loop
   while(!glfwWindowShouldClose(MainWindow))
    {
      float CurrentFrame = glfwGetTime();
      GreenValue = (sin(CurrentFrame) / 2.0f) + 0.5f;
      DeltaTime = CurrentFrame - LastFrame;
      LastFrame = CurrentFrame;

      ProcessInput(MainWindow);

      
      glm::mat4 View = MyCamera.GetViewMatrix();
      glBindBuffer(GL_UNIFORM_BUFFER, UBOMat);
      glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4),
		      sizeof(glm::mat4), glm::value_ptr(View));
      glBindBuffer(GL_UNIFORM_BUFFER, 0);
      
      
      // glBindFramebuffer(GL_FRAMEBUFFER, FBOFront);
      // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld1(CubeShader, LightShader, CubeMapShader);      

      // glBindFramebuffer(GL_FRAMEBUFFER, FBOBack);
      // glClearColor(0.25f, 0.12f, 0.12f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld2(CubeShader, LightShader);      

      // glBindFramebuffer(GL_FRAMEBUFFER, FBOLeft);
      // glClearColor(0.12f, 0.25f, 0.12f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld3(CubeShader, LightShader);      
      
      // glBindFramebuffer(GL_FRAMEBUFFER, FBORight);
      // glClearColor(0.12f, 0.12f, 0.32f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld4(CubeShader, LightShader, CubeMapShader);      

      // glBindFramebuffer(GL_FRAMEBUFFER, FBOTop);
      // glClearColor(0.12f, 0.32f, 0.32f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld5(CubeShader, LightShader);      

      // glBindFramebuffer(GL_FRAMEBUFFER, FBOBottom);
      // glClearColor(0.68f, 0.55f, 0.12f, 1.0f);
      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      
      // RenderWorld6(CubeShader, ModelMesh);      
      
      
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClearColor(0.54f, 0.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      CubeShader.Use();
      CubeShader.SetMat4("Model", glm::mat4(1.0f));
      CubeShader.SetMat4("View", MyCamera.GetViewMatrix());      
      glBindVertexArray(VAO[1]);	      	      
      glDrawArrays(GL_TRIANGLES, 0, 36);



      // RenderMainWorld(QuadShader, CubeMapShader);

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



void InitCubeMap(unsigned int* CubeID, std::vector<char*> FaceTextures)
{
  glGenTextures(1, CubeID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, *CubeID);

  int ImgWidth;
  int ImgHeight;
  int ImgChannels;
  for(unsigned int i = 0; i < FaceTextures.size(); ++i)
    {
      unsigned char* data = stbi_load(FaceTextures[i], &ImgWidth, &ImgHeight,
				      &ImgChannels, 0);
      if(data)
	{
	  glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, ImgWidth,
		       ImgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
		       data);
	  glGenerateMipmap(GL_TEXTURE_2D);
	}
      else
	{
	  printf("SPI_ERROR -> Failed to load texture in CubeMap at path "
		 "%s\nWill render wierdly\n", FaceTextures[i]);
	  stbi_image_free(data);
	}
    }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void RenderWorld1(Shader CubeShader, Shader LightShader,
		  Shader CubeMapShader)
{
  glm::vec3 CubePos[] =
    {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
    };

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, CubePos[0]);

  CubeShader.Use();      
  CubeShader.SetMat4("Model", Model);	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  glBindVertexArray(VAO[1]);	      	      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  CubeShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);


  CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
		       0.35f, 1.0f);
  glBindVertexArray(VAO[0]);
  for(int i = 0; i < 4; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      LightPosArray[i].z = GreenValue * 2.0f;
      LightPosArray[i].y = GreenValue * 2.0f;
      LightPosArray[i].x = GreenValue * 5.0f;
      Model = glm::translate(Model, LightPosArray[i]);
      LightShader.Use();
      LightShader.SetMat4("View", MyCamera.GetViewMatrix());
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 55.0f),
			  glm::vec3(6.14f, 6.36f, 6.52f));
      Model = glm::translate(Model, LightPosArray[i]);	      
      Model = glm::scale(Model, glm::vec3(0.2));
      LightShader.SetMat4("Model", Model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  CubeShader.Use();

  glBindVertexArray(VAO[1]);	      
      
  for(int i = 0; i < 10; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::translate(Model, CubePos[i]);
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
	      
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  glDepthFunc(GL_LEQUAL);
  CubeMapShader.Use();
  glm::mat4 View = glm::mat4(glm::mat3(MyCamera.GetViewMatrix()));
  CubeMapShader.SetMat4("View", View);
  glBindVertexArray(VAO[0]);
  glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_TRUE);
}

void RenderWorld2(Shader CubeShader, Shader LightShader)
{
  glm::vec3 CubePos[] =
    {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
    };

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, CubePos[0]);

  CubeShader.Use();      

  CubeShader.SetMat4("Model", Model);	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  glBindVertexArray(VAO[1]);	      	      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  CubeShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);


  CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
		       0.35f, 1.0f);
  glBindVertexArray(VAO[0]);
  for(int i = 0; i < 4; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      LightPosArray[i].z = GreenValue * 10.0f;
      LightPosArray[i].x = GreenValue * 2.0f;
      Model = glm::translate(Model, LightPosArray[i]);
      LightShader.Use();
      LightShader.SetMat4("View", MyCamera.GetViewMatrix());
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 55.0f),
			  glm::vec3(6.14f, 6.36f, 6.52f));
      Model = glm::translate(Model, LightPosArray[i]);	      
      Model = glm::scale(Model, glm::vec3(0.2));
      LightShader.SetMat4("Model", Model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  CubeShader.Use();

  glBindVertexArray(VAO[1]);	      
      
  for(int i = 0; i < 10; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 25.0f),
			  glm::vec3(0.14f, 2.36f, 2.52f));      
      Model = glm::translate(Model, CubePos[i]);
    
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
	      
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void RenderWorld3(Shader CubeShader, Shader LightShader)
{
  glm::vec3 CubePos[] =
    {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
    };

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, CubePos[0]);

  CubeShader.Use();      

  CubeShader.SetMat4("Model", Model);	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  glBindVertexArray(VAO[1]);	      	      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  CubeShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);


  CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
		       0.35f, 1.0f);
  glBindVertexArray(VAO[0]);
  for(int i = 0; i < 4; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      LightPosArray[i].z = GreenValue * 10.0f;
      LightPosArray[i].x = GreenValue * 2.0f;
      Model = glm::translate(Model, LightPosArray[i]);
      LightShader.Use();
      LightShader.SetMat4("View", MyCamera.GetViewMatrix());
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 55.0f),
			  glm::vec3(6.14f, 6.36f, 6.52f));
      Model = glm::translate(Model, LightPosArray[i]);	      
      Model = glm::scale(Model, glm::vec3(0.2));
      LightShader.SetMat4("Model", Model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  CubeShader.Use();

  glBindVertexArray(VAO[1]);	      
      
  for(int i = 0; i < 10; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::translate(Model, CubePos[i]);
      Model = glm::scale(Model, glm::vec3(GreenValue,0.5f - GreenValue, GreenValue));
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 25.0f),
			  glm::vec3(0.14f, 2.36f, 2.52f));
    
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
	      
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }  
}
void RenderWorld4(Shader CubeShader, Shader LightShader,
		  Shader CubeMapShader)
{
  glm::vec3 CubePos[] =
    {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
    };

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, CubePos[0]);

  CubeShader.Use();      

  CubeShader.SetMat4("Model", Model);	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  glBindVertexArray(VAO[1]);	      	      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  CubeShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);


  CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
		       0.35f, 1.0f);
  glBindVertexArray(VAO[0]);
  for(int i = 0; i < 4; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      LightPosArray[i].z = GreenValue * 2.0f;
      LightPosArray[i].y = GreenValue * 2.0f;
      LightPosArray[i].x = GreenValue * 5.0f;
      Model = glm::translate(Model, LightPosArray[i]);
      LightShader.Use();
      LightShader.SetMat4("View", MyCamera.GetViewMatrix());
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 55.0f),
			  glm::vec3(6.14f, 6.36f, 6.52f));
      Model = glm::translate(Model, LightPosArray[i]);	      
      Model = glm::scale(Model, glm::vec3(0.2));
      LightShader.SetMat4("Model", Model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  CubeShader.Use();

  glBindVertexArray(VAO[1]);	      
      
  for(int i = 0; i < 3; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::scale(Model, glm::vec3(GreenValue * 4.0));
      Model = glm::translate(Model, CubePos[i]);
    
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);
	      
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  glDepthFunc(GL_LEQUAL);
  CubeMapShader.Use();
  CubeMapShader.SetVec4("TextureMult", glm::vec4(55.0f, 0.0f, 0.0f, 1.0f));
  glm::mat4 View = glm::mat4(glm::mat3(MyCamera.GetViewMatrix()));
  CubeMapShader.SetMat4("View", View);
  glBindVertexArray(VAO[0]);
  glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  CubeMapShader.SetVec4("TextureMult", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
  glDepthFunc(GL_TRUE);
}

void RenderWorld5(Shader CubeShader, Shader LightShader)
{
  glm::vec3 CubePos[] =
    {
      glm::vec3( 0.0f, 0.0f, 0.0f),
      glm::vec3( 2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f),
      glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3( 2.4f, -0.4f, -3.5f),
      glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3( 1.3f, -2.0f, -2.5f),
      glm::vec3( 1.5f, 2.0f, -2.5f),
      glm::vec3( 1.5f, 0.2f, -1.5f),
      glm::vec3(-1.3f, 1.0f, -1.5f)
    };

  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, CubePos[0]);

  CubeShader.Use();      

  CubeShader.SetMat4("Model", Model);	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  glBindVertexArray(VAO[1]);	      	      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);

  CubeShader.Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RegTex[0]);


  CubeShader.SetFloat4("UniColor", 0.1f, GreenValue,
		       0.35f, 1.0f);
  glBindVertexArray(VAO[0]);
  for(int i = 0; i < 4; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      LightPosArray[i].z = GreenValue * 2.0f;
      LightPosArray[i].y = GreenValue * 2.0f;
      LightPosArray[i].x = GreenValue * 5.0f;
      Model = glm::translate(Model, LightPosArray[i]);
      LightShader.Use();
      LightShader.SetMat4("View", MyCamera.GetViewMatrix());
      Model = glm::rotate(Model,
			  glm::radians(GreenValue * 55.0f),
			  glm::vec3(6.14f, 6.36f, 6.52f));
      Model = glm::translate(Model, LightPosArray[i]);	      
      Model = glm::scale(Model, glm::vec3(0.2));
      LightShader.SetMat4("Model", Model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

  CubeShader.Use();

  glBindVertexArray(VAO[1]);	      
      
  for(int i = 0; i < 10; ++i)
    {
      glm::mat4 Model = glm::mat4(1.0f);
      Model = glm::scale(Model, glm::vec3(GreenValue * 4.0 + 1.0f));
      Model = glm::rotate(Model, glm::radians(22.0f), glm::vec3(GreenValue, GreenValue, 2.0f-GreenValue));
      Model = glm::translate(Model, CubePos[i]);
    
      CubeShader.SetMat4("Model", Model);
	  
      CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
      CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
      CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
      CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
      CubeShader.SetVec3("SpotLight.Position",
			 MyCamera.Position);
      CubeShader.SetVec3("CameraPos", MyCamera.Position);
      CubeShader.SetVec3("SpotLight.Direction",
			 MyCamera.Front);

      for(int j = 0; j < 8; j++)
	{
	  glDrawArrays(GL_TRIANGLES, j * 2, 6);
	}
    }
}

void RenderWorld6(Shader CubeShader, Model ModelMesh)
{
  CubeShader.Use();
      
  glm::mat4 Model = glm::mat4(1.0f);
  Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -5.0f));
  CubeShader.SetMat4("View", glm::mat4(1.0f));
  CubeShader.SetMat4("Model", Model);
	  
  CubeShader.SetVec3("PointLights[0].Position", LightPosArray[0]);
  CubeShader.SetVec3("PointLights[1].Position", LightPosArray[1]);
  CubeShader.SetVec3("PointLights[2].Position", LightPosArray[2]);
  CubeShader.SetVec3("PointLights[3].Position", LightPosArray[3]);
	  
  CubeShader.SetVec3("SpotLight.Position",
		     MyCamera.Position);
  CubeShader.SetVec3("CameraPos", MyCamera.Position);
  CubeShader.SetVec3("SpotLight.Direction",
		     MyCamera.Front);

  ModelMesh.Draw(CubeShader);  
}

void RenderMainWorld(Shader QuadShader, Shader CubeMapShader)
{
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 View = glm::mat4(glm::mat3(MyCamera.GetViewMatrix()));
  QuadShader.Use();
  QuadShader.SetMat4("View", MyCamera.GetViewMatrix());
  Model = glm::mat4(1.0f);
  Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -10.0f));
  Model = glm::rotate(Model, (float)glfwGetTime() / 2.0f, glm::vec3(5.0f, 2.0f, 5.0f));  
  Model = glm::scale(Model, glm::vec3(3.0f, 3.0f, 1.0f));
  // Model = glm::scale(Model, glm::vec3(GreenValue * 5.0f));
  QuadShader.SetMat4("Model", Model);
  glActiveTexture(GL_TEXTURE0);
  
  //Front
  glBindVertexArray(WorldFrontVAO);
  glBindTexture(GL_TEXTURE_2D, WorldFrontTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      

  //Back
  glBindVertexArray(WorldBackVAO);
  glBindTexture(GL_TEXTURE_2D, WorldBackTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      

  //Left
  glBindVertexArray(WorldLeftVAO);
  glBindTexture(GL_TEXTURE_2D, WorldLeftTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      

  //Right
  glBindVertexArray(WorldRightVAO);
  glBindTexture(GL_TEXTURE_2D, WorldRightTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      

  //Top
  glBindVertexArray(WorldTopVAO);
  glBindTexture(GL_TEXTURE_2D, WorldTopTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      
  
  //Bottom
  glBindVertexArray(WorldBottomVAO);
  glBindTexture(GL_TEXTURE_2D, WorldBottomTexture);
  glDrawArrays(GL_TRIANGLES, 0, 6);      
  
  
  glDepthFunc(GL_LEQUAL);
  CubeMapShader.Use();
  CubeMapShader.SetMat4("View", View);
  glBindVertexArray(VAO[0]);
  glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMap);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glDepthFunc(GL_TRUE);
}

