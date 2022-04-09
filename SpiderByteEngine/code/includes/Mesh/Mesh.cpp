#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Mesh.h"
#include "../Shader/Shader.h"
#include <stdio.h>
#include <vector>
#include <string.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
  this->Vertices = vertices;
  this->Indices = indices;
  this->Textures = textures;

  SetUpMesh();
}

void Mesh::SetUpMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex),
	       &Vertices[0], GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() *
	       sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)0);
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, Normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, Position));
  
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
			(void*)offsetof(Vertex, TexCoords));

  glBindVertexArray(0);
}


//@NOTE(Emilio): Function used std::string
//@TODO(Emilio): May need custom string functions for char*
void Mesh::Draw(Shader& shader)
{
  unsigned int DiffuseNum = 1;
  unsigned int SpecularNum = 1;

  for(unsigned int i = 0; i < Textures.size(); ++i)
    {
      glActiveTexture(GL_TEXTURE + i);
      std::string Number;
      std::string Name = Textures[i].Type;
      if(Name == "Diffuse")
	{
	  Number = std::to_string(DiffuseNum++);	  
	}
      else
	{
	  Number = std::to_string(SpecularNum++);
	}
      shader.SetFloat((char*)("Material." + Name + Number).c_str(), i);
      glBindTexture(GL_TEXTURE_2D, Textures[i].ID - 1);//@NOTE(Emilio): <- (ID - 1) is weird!!?      
    }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);  
}
