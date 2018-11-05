#pragma once
#define GL_GLEXT_PROTOTYPES 1

#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#define printGLError() if(glGetError() != 0) std::cout<<"GL error: "<<glGetError()<<std::endl



class openglshader
{
	
private:
  GLuint shaderId;
  
  GLchar* shaderSource;
  
  GLuint shaderType;

  std::ifstream* shaderFile;

public:

  openglshader() : shaderId(0), shaderSource(NULL), shaderType(0), shaderFile(NULL) {}
  void loadShader(GLuint shaderType, const char* filename);
  GLuint getId() {return shaderId;}
  void printLog();
  
  void change_mat4(GLint programID, const char* uniform_name, glm::mat4 matrix);
  void configure_mat4(GLint programID, const char* uniform_name, glm::mat4 matrix);
  void configure_mat3(GLint programID, const char* uniform_name, glm::mat3 matrix);
  void configure_texture(GLint programID, const char* uniform_sampler2D_name, unsigned char* texture, GLsizei array_texture_size);
  void change_float(GLint programID, const char* uniform_name, float value);
  void configure_attributes(GLint programID, const char* attribute_name, const GLvoid* buffer_data, GLint elements_per_Vertex);
  
  ~openglshader() { if(shaderSource) delete[] shaderSource; shaderFile->close();}
  

private:
  GLchar* readFile(int* fileSize);
  int countFileSize(std::istream* shaderFile);
  
};




class openglprogram
{

private:
  openglshader* vertexShader;
  
  openglshader* fragmentShader;
  
  GLuint programId;

public:
  openglprogram(): vertexShader(NULL), fragmentShader(NULL), programId(-1) {}
  
  void printProgramLog();
  
  void createShaders(const char* vertexShaderFile, const char* fragmentShaderFile);
  
  GLuint getId() { return programId;}
  
  ~openglprogram() {if(vertexShader) delete vertexShader; if(fragmentShader) delete fragmentShader; }
};
