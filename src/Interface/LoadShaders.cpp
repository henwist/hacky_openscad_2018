
#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>

#include <assert.h>

#include "LoadShaders.h"



void openglprogram::printProgramLog()
{
  
   	GLint logLength;
	GLint actualLogLength;
	
	
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
	
	char* programMessage = new char[logLength];
	
	
	glGetProgramInfoLog(programId, logLength, &actualLogLength, programMessage);

	std::cout << "ProgramLog: ";
	
	for(int i=0; i<actualLogLength; i++)
	  std::cout << programMessage[i];
	
	vertexShader->printLog();
	fragmentShader->printLog();
	
	std::cout << std::endl; 
	
 
}


void openglprogram::createShaders(const char* vertexShaderFile, const char* fragmentShaderFile)
{

        programId = glCreateProgram();

	vertexShader =   new openglshader();
	fragmentShader = new openglshader();
	
        assert(vertexShader);
	assert(fragmentShader);
	
	vertexShader->loadShader(GL_VERTEX_SHADER, vertexShaderFile);
	fragmentShader->loadShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
	
	glAttachShader(programId, vertexShader->getId());
	printGLError();

	
        glAttachShader(programId, fragmentShader->getId());
	printGLError();
	
        glLinkProgram(programId);
	printGLError();
	
	vertexShader->printLog();
	fragmentShader->printLog();
	
	printProgramLog();

}


int openglshader::countFileSize(std::istream* shaderFile)
{
  
  int fileSize = 0;
    
  while(shaderFile->good())
  {
    shaderFile->get();
    
    if(shaderFile->eof() != std::ifstream::eofbit)
      fileSize++;
  } 
  
  return fileSize;
}


GLchar* openglshader::readFile(int* fileSize)
{
  int reqFileSize = *fileSize;
  
  GLchar* shaderSource = new GLchar [reqFileSize + 1];
    
  for(int i=0; i<reqFileSize; i++)
  {
    if(shaderFile->good())
      *shaderFile >> std::noskipws >> shaderSource[i];
    
    if(shaderFile->eof() || shaderFile->bad())
    {
      *fileSize = i;
      shaderSource[i] = '\0';
      break;
    }
  } 
  
  shaderSource[reqFileSize] = '\0';
  
  return shaderSource;
}



void openglshader::loadShader(GLuint shaderType, const char* filename)
{

  int fileSize = 0;
  
  this->shaderType = shaderType;
  
  shaderFile = new std::ifstream(filename, std::ifstream::in);
  
  assert(shaderFile);
  
  
  fileSize = countFileSize(shaderFile);

  
  std::cout << "fileSize: " << fileSize << "\n";
  
  //Rewind file to startposition.
  shaderFile->clear();
  shaderFile->seekg(0);
  
  this->shaderSource = readFile(&fileSize);
  

    
  //shaderFile->get(shaderSource, fileSize);
  
//   for(int i=0; i<fileSize; i++)
//     std::cout << this->shaderSource[i];
  
  std::cout << "\n";
  
  
  shaderId = glCreateShader(shaderType);
  
  std::cout << __FILE__ << ": Id: " << shaderId << std::endl; 
  
  
  glShaderSource(shaderId, 1, (const GLchar**)&shaderSource, 0);
  printGLError();
  
  
  glCompileShader(shaderId);
  printGLError();
  
  
  printLog();
}



void openglshader::printLog() //GL_FRAGMENT_SHADER, GL_VERTEX_SHADER
{
 	GLint logLength;
	GLint actualLogLength;
	
	
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	
	char* shaderMessage = new char[logLength];
	
	
	glGetShaderInfoLog(shaderId, logLength, &actualLogLength, shaderMessage);
	
	switch(shaderType)
	{
	  case GL_VERTEX_SHADER:
	    std::cout << "VertexShaderLog: ";
	    break;
	    
	  case GL_FRAGMENT_SHADER:
	    std::cout << "FragmentShader: ";
	    break;
	    
	  default:
	    std::cout << "No shader could be determined." << std::endl;
	}
	
	for(int i=0; i<actualLogLength; i++)
	  std::cout << shaderMessage[i];
	
	std::cout << std::endl; 
}

void openglshader::change_mat4(GLint programID, const char* uniform_name, glm::mat4 matrix)
{
  GLint params;
  GLint uniLoc_matrix;
  
  //configure matrices.
  uniLoc_matrix = glGetUniformLocation(programID, uniform_name);
    
  glUniformMatrix4fv( uniLoc_matrix, 1, GL_FALSE, &matrix[0][0]);
}




void openglshader::configure_mat4(GLint programID, const char* uniform_name, glm::mat4 matrix)
{
  GLint params;
  GLint uniLoc_matrix;


  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &params);
  //std::cout << "active uniforms count: " << params << std::endl;

  
  //configure matrices.
  uniLoc_matrix = glGetUniformLocation(programID, uniform_name);
  //std::cout << "ret from uniforms location = " << uniLoc_matrix << std::endl;
    
  glUniformMatrix4fv( uniLoc_matrix, 1, GL_FALSE, &matrix[0][0]);
}


void openglshader::configure_mat3(GLint programID, const char* uniform_name, glm::mat3 matrix)
{
  GLint params;
  GLint uniLoc_matrix;


  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &params);
  std::cout << "active uniforms count: " << params << std::endl;

  
  //configure matrices.
  uniLoc_matrix = glGetUniformLocation(programID, uniform_name);
  std::cout << "ret from uniforms location = " << uniLoc_matrix << std::endl;
    
  glUniformMatrix3fv( uniLoc_matrix, 1, GL_FALSE, &matrix[0][0]);
}




void openglshader::configure_texture(GLint programID, const char* uniform_sampler2D_name, unsigned char* texture, GLsizei array_texture_size)
{
  GLint params;
  GLint uniLoc_matrix;


  glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &params);
  std::cout << "active uniforms count: " << params << std::endl;

  
  //configure matrices.
  uniLoc_matrix = glGetUniformLocation(programID, uniform_sampler2D_name);
  std::cout << "ret from uniforms location = " << uniLoc_matrix << std::endl;
    
  glUniform1i(uniLoc_matrix, 0); //
  //glUniform1iv( uniLoc_matrix, array_texture_size, (const GLint*)texture); //used for array of samplers - dont know how this works though...
  //glBindSampler(unit​, uniLoc_matrix); //glBindSampler(GLuint unit​, GLuint uniLoc_matrix);
}


void openglshader::change_float(GLint programID, const char* uniform_name, float value)
{
  GLint params;
  GLint location;
  
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &params);
  //std::cout << "active uniforms count: " << params << std::endl;
  
  location = glGetUniformLocation(programID, uniform_name);
    //std::cout << "ret from uniforms location = " << location << std::endl;
    
  glUniform1f( location, value);
}





void openglshader::configure_attributes(GLint programID, const char* attribute_name, const GLvoid* buffer_data, GLint elements_per_Vertex)
{
  
  GLint attrib_Loc_index;
  
  //configure arributes
  attrib_Loc_index = glGetAttribLocation(programID, attribute_name);
		
  glVertexAttribPointer(
	  attrib_Loc_index, //vertexPosition_modelspaceID, // The attribute we want to configure
	  elements_per_Vertex,                  // size: 1, 2, 3 or 4 alements per generic Vertex.
	  GL_FLOAT,           // type
	  GL_FALSE,           // normalized?
	  0,                  // stride
	  buffer_data // (void*)0            // array buffer offset
  );

  glEnableVertexAttribArray ( attrib_Loc_index );
  
}
