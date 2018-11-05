#pragma once

#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <boost/shared_ptr.hpp>

#include <QtGui/QOpenGLFunctions>

#define printGLError() if(glGetError() != 0) std::cout<<"GL error: "<<glGetError()<<std::endl



class openglshader : protected QOpenGLFunctions
{
	
private:
  GLuint shaderId;
  
  boost::shared_ptr<GLchar[]> shaderSource;
  
  GLuint shaderType;

  boost::shared_ptr<std::ifstream> shaderFile;

public:

  openglshader() : shaderId(0), shaderSource((GLchar*)NULL), shaderType(0), shaderFile((std::ifstream*)NULL) { }
  void loadShader(GLuint shaderType, const char* filename);
  GLuint getId() {return shaderId;}
  void printLog();
  
  ~openglshader() { /*if(shaderSource) delete[] shaderSource;*/ /*shaderFile->close();*/}
  

private:
  boost::shared_ptr<GLchar[]> readFile(int* fileSize);
  int countFileSize(boost::shared_ptr<std::ifstream> shaderFile);
  
};




class openglprogram : protected QOpenGLFunctions
{

private:
  boost::shared_ptr<openglshader> vertexShader;
  
  boost::shared_ptr<openglshader> fragmentShader;
  
  GLuint programId;

public:
  openglprogram(): vertexShader((openglshader*)NULL), fragmentShader((openglshader*)NULL), programId(-1) { }
  
  void printProgramLog();
  
  void createShaders(const char* vertexShaderFile, const char* fragmentShaderFile);
  
  GLuint getId() { return programId;}
  void change_mat4(const char* uniform_name, glm::mat4 matrix);
  void configure_mat4(const char* uniform_name, glm::mat4 matrix);
  void configure_mat3(const char* uniform_name, glm::mat3 matrix);
  void configure_texture(const char* uniform_sampler2D_name, unsigned char* texture, GLsizei array_texture_size);
  void change_float(const char* uniform_name, float value);
  void configure_attributes(const char* attribute_name, const GLvoid* buffer_data, GLint elements_per_Vertex);
  
  ~openglprogram() {/*if(vertexShader) delete vertexShader; if(fragmentShader) delete fragmentShader;*/ }
};
