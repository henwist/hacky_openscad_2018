#pragma once

  #define GL_GLEXT_PROTOTYPES 1
  #define GLM_FORCE_RADIANS
  #define EPSILON 0.000001



  #define WINDOW_WIDTH  480
  #define WINDOW_HEIGHT 360
  #define WINDOW_ASPECT_RATIO  WINDOW_WIDTH /  WINDOW_HEIGHT

  #define OPENGL_WORLD_COORDINATE_X 100.0/20.0
  #define OPENGL_WORLD_COORDINATE_Y 100.0/20.0
  #define OPENGL_WORLD_NEAR -1000.05 //negative working? hmm...
  #define OPENGL_WORLD_FAR 200.0


  #define UNILOC_ROTMATRIX  "rotMatrix"
  #define UNILOC_M  "m" //model matrix
  #define UNILOC_V  "v" //view matrix
  #define UNILOC_P  "p" //perspective matrix

  #define UNILOC_V_INV "v_inv"
  #define UNILOC_MV_INV "mv_inv"

  #define UNILOC_ALPHA "alpha"

  #define ATTRIB_VERTEX "vertex"
  #define ATTRIB_NORMAL "normal"


  //These values will be used per facet when constructing data buffers to render.
  #define NUM_VALUES_PER_NORMAL  3
  #define NUM_VALUES_PER_VERTEX  4

  
  //used for setting the axis of rotation
  #define XAXIS 0
  #define YAXIS 0
  #define ZAXIS 1

  //rotate the axis in this speed.
  #define ROT_SPEED 0.05
  #define ZOOM_FACTOR 5 



#include <iostream>
#include <assert.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include <QtGui/QOpenGLFunctions>

class view : protected QOpenGLFunctions
{
  
public:
  view();
  
  void normalizedDeviceCoordinates(int screenX, int screenY, float scale,
				 float* normalizedX, float* normalizedY, glm::vec3* normalizedCursorPosition);
  
  glm::mat4 getProjectionMatrix();
  glm::mat4 getViewMatrix();
  
  ~view();
  
private:
  int width;
  int height;

  glm::mat4 p;
  glm::mat4 v;
  
  glm::vec3 cameraPosition;
  glm::vec3 cameraTarget;
  glm::vec3 upVector;

  void init();

  void initMatrices();
  void setupCamera();
};