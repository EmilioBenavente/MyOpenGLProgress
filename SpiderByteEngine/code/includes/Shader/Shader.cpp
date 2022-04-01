#include "Shader.h"
#include <stdio.h>
#include <glad/glad.h>
#include <fstream>
#include <sstream>

Shader::Shader(const char* VertPath, const char* FragPath)
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
  int SUCCESS;
  char InfoLog[512];

  Vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(Vertex, 1, &VertShaderCode, 0);
  glCompileShader(Vertex);
  glGetShaderiv(Vertex, GL_COMPILE_STATUS, &SUCCESS);
  if(!SUCCESS)
    {
      glGetShaderInfoLog(Vertex, 512, 0, InfoLog);
      printf("SPI_ERROR -> Unable to Compile the Vertex Shader With Given Path %s\nand ID[%d]\n", VertPath, Vertex);
    }
  Fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(Fragment, 1, &FragShaderCode, 0);
  glCompileShader(Fragment);
  glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SUCCESS);
  if(!SUCCESS)
    {
      glGetShaderInfoLog(Fragment, 512, 0, InfoLog);
      printf("SPI_ERROR -> Unable to Compile the Fragment Shader With Given Path %s\nand ID[%d]\n", FragPath, Fragment);
    }
  PROGRAM_ID = glCreateProgram();
  glAttachShader(PROGRAM_ID, Vertex);
  glAttachShader(PROGRAM_ID, Fragment);
  glLinkProgram(PROGRAM_ID);
  glDeleteShader(Vertex);
  glDeleteShader(Fragment);
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
