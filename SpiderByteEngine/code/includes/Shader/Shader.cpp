#include "Shader.h"
#include <stdio.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const char* VertPath, const char* FragPath, const char* GeometryPath)
{
  std::string VertexCode;
  std::string FragmentCode;
  std::ifstream VertFileBuffer;
  std::ifstream FragFileBuffer;
  VertFileBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  FragFileBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
    {
      VertFileBuffer.open(VertPath);
      FragFileBuffer.open(FragPath);

      std::stringstream VertShaderStream;
      std::stringstream FragShaderStream;

      VertShaderStream << VertFileBuffer.rdbuf();
      FragShaderStream << FragFileBuffer.rdbuf();

      VertFileBuffer.close();
      FragFileBuffer.close();

      VertexCode = VertShaderStream.str();
      FragmentCode = FragShaderStream.str();
    }
  catch(std::ifstream::failure e)
    {
      printf("SPI_ERROR -> Unable to extract data from ShaderFiles with given path\n"
             "Vertex Shader Path -> %s\nFragment Shader Path -> %s\n", VertPath, FragPath);
    }
  
  const char* VertShaderCode = VertexCode.c_str();
  const char* FragShaderCode = FragmentCode.c_str();

  unsigned int Vertex;
  unsigned int Fragment;
  unsigned int Geometry;
  
  int SUCCESS;
  char InfoLog[512];

  Vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(Vertex, 1, &VertShaderCode, 0);
  glCompileShader(Vertex);
  glGetShaderiv(Vertex, GL_COMPILE_STATUS, &SUCCESS);
  if(!SUCCESS)
    {
      glGetShaderInfoLog(Vertex, 512, 0, InfoLog);
      printf("SPI_ERROR -> Unable to Compile the Vertex Shader With Given Path %s\nand ID[%d]\n%s\n", VertPath, Vertex, InfoLog);
    }
  Fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(Fragment, 1, &FragShaderCode, 0);
  glCompileShader(Fragment);
  glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SUCCESS);
  if(!SUCCESS)
    {
      glGetShaderInfoLog(Fragment, 512, 0, InfoLog);
      printf("SPI_ERROR -> Unable to Compile the Fragment Shader With Given Path %s\nand ID[%d]\n%s\n", FragPath, Fragment, InfoLog);
    }


  if(GeometryPath)
    {
      std::string GeometryCode;
      std::ifstream GeometryFileBuffer;
      GeometryFileBuffer.exceptions(std::ifstream::failbit | std::ifstream::badbit);

      try
	{
	  GeometryFileBuffer.open(GeometryPath);
	  std::stringstream GeoShaderStream;
	  GeoShaderStream << GeometryFileBuffer.rdbuf();
	  GeometryFileBuffer.close();
	  GeometryCode = GeoShaderStream.str();
	}
      catch(std::ifstream::failure e)
	{
	  printf("SPI_ERROR -> Unable to extract data from Geometry ShaderFile"
		 "with path\n%s\n", GeometryPath);
	}
      const char* GeoShaderCode = GeometryCode.c_str();

      Geometry = glCreateShader(GL_GEOMETRY_SHADER);
      glShaderSource(Geometry, 1, &GeoShaderCode, 0);
      glCompileShader(Geometry);
      glGetShaderiv(Geometry, GL_COMPILE_STATUS, &SUCCESS);
      if(!SUCCESS)
	{
	  glGetShaderInfoLog(Geometry, 512, 0, InfoLog);
	  printf("SPI_ERROR -> Unable to Compile the Geometry Shader With Given Path %s\nand ID[%d]\n%s\n", GeometryPath, Geometry, InfoLog);
	}

    }
  PROGRAM_ID = glCreateProgram();
  glAttachShader(PROGRAM_ID, Vertex);
  glAttachShader(PROGRAM_ID, Fragment);
  if(GeometryPath)
    {
      glAttachShader(PROGRAM_ID, Geometry);
    }
  glLinkProgram(PROGRAM_ID);
  glDeleteShader(Vertex);
  glDeleteShader(Fragment);
  if(GeometryPath)
    {
      glDeleteShader(Geometry);
    }  
}

void Shader::Use()
{
  glUseProgram(PROGRAM_ID);
}

void Shader::SetBool(char* Name, bool A)
{
  glUniform1i(glGetUniformLocation(PROGRAM_ID, Name), (int)A);
}

void Shader::SetBool2(char* Name, bool A, bool B)
{
  glUniform2i(glGetUniformLocation(PROGRAM_ID, Name), (int)A, (int)B);
}

void Shader::SetBool3(char* Name, bool A, bool B, bool C)
{
  glUniform3i(glGetUniformLocation(PROGRAM_ID, Name), (int)A, (int)B, (int)C);
}

void Shader::SetBool4(char* Name, bool A, bool B, bool C, bool D)
{
  glUniform4i(glGetUniformLocation(PROGRAM_ID, Name), (int)A, (int)B, (int)C,(int)D);
}


void Shader::SetInt(char* Name, int A)
{
  glUniform1i(glGetUniformLocation(PROGRAM_ID, Name), A);
}

void Shader::SetInt2(char* Name, int A, int B)
{
  glUniform2i(glGetUniformLocation(PROGRAM_ID, Name), A, B);
}

void Shader::SetInt3(char* Name, int A, int B, int C)
{
  glUniform3i(glGetUniformLocation(PROGRAM_ID, Name), A, B, C);
}

void Shader::SetInt4(char* Name, int A, int B, int C, int D)
{
  glUniform4i(glGetUniformLocation(PROGRAM_ID, Name), A, B, C, D);
}

void Shader::SetFloat(char* Name, float A)
{
  glUniform1f(glGetUniformLocation(PROGRAM_ID, Name), A);
}

void Shader::SetFloat2(char* Name, float A, float B)
{
  glUniform2f(glGetUniformLocation(PROGRAM_ID, Name), A, B);
}

void Shader::SetFloat3(char* Name, float A, float B, float C)
{
  glUniform3f(glGetUniformLocation(PROGRAM_ID, Name), A, B, C);
}

void Shader::SetFloat4(char* Name, float A, float B, float C, float D)
{
  glUniform4f(glGetUniformLocation(PROGRAM_ID, Name), A, B, C, D);
}

void Shader::SetVec2(char* Name, glm::vec2 Value)
{
  glUniform2f(glGetUniformLocation(PROGRAM_ID, Name), Value.x, Value.y);
}

void Shader::SetVec3(char* Name, glm::vec3 Value)
{
  glUniform3f(glGetUniformLocation(PROGRAM_ID, Name), Value.x, Value.y, Value.z);
}

void Shader::SetVec4(char* Name, glm::vec4 Value)
{
  glUniform4f(glGetUniformLocation(PROGRAM_ID, Name), Value.x, Value.y, Value.z, Value.w);
}

void Shader::SetMat2(char* Name, glm::mat2 Value)
{
  glUniformMatrix2fv(glGetUniformLocation(PROGRAM_ID, Name), 1, GL_FALSE, glm::value_ptr(Value));
}

void Shader::SetMat3(char* Name, glm::mat3 Value)
{
  glUniformMatrix3fv(glGetUniformLocation(PROGRAM_ID, Name), 1, GL_FALSE, glm::value_ptr(Value));
}

void Shader::SetMat4(char* Name, glm::mat4 Value)
{
  glUniformMatrix4fv(glGetUniformLocation(PROGRAM_ID, Name), 1, GL_FALSE, glm::value_ptr(Value));
}

void Shader::SetUniformBlockBinding(char* Name, int Value)
{
  glUniformBlockBinding(PROGRAM_ID, glGetUniformBlockIndex(PROGRAM_ID, Name), Value);
}
