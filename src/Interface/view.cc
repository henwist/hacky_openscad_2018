#include "view.hpp"

view::view()
{  
  init();

  initMatrices();  
}



view::~view()
{

}


void view::normalizedDeviceCoordinates(int screenX, int screenY, float scale,
				 float* normalizedX, float* normalizedY, glm::vec3* normalizedCursorPosition)
{
  
  *normalizedX =   (((float)screenX / WINDOW_WIDTH)  * 2.0 - 1.0) * OPENGL_WORLD_COORDINATE_X * WINDOW_ASPECT_RATIO / scale;
  
  *normalizedY = -(((float)screenY / WINDOW_HEIGHT) * 2.0 - 1.0) * OPENGL_WORLD_COORDINATE_Y / scale;
  
  *normalizedCursorPosition = glm::vec3(*normalizedX, *normalizedY, 0.0);

  std::cout << "normalizedCursorPosition: " << glm::to_string(*normalizedCursorPosition) << "\n" << "\n";
}

glm::mat4 view::getProjectionMatrix()
{
    return p;
}

glm::mat4 view::getViewMatrix()
{
    return v;
}

void view::init()
{
  int x = 0;
  int y = 0;
  width = 640;
  height = 480;

 initializeOpenGLFunctions();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	//glEnable(GL_SMOOTH);
// 	glEnable(GL_RGBA);
// 	glEnable(GL_LINE_SMOOTH);
// 	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND); //used to be able to use alpha in fragment shader.
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST ); 
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST ); 
	//glEnable(GL_TEXTURE_2D); //is not used in gl es2
	//glEnable( GL_RGBA); //not working hmm I dont know why...
	
	//glEnable(GL_MULTISAMPLE_ARB);
// 	glEnable(GL_DOUBLE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LESS); 
	glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport ( x, y, width, height ); //is working very well -> the object is inn one corner and smaller.  
} 

void view::initMatrices()
{
 	//configure matrices - projection matrix.
	p = glm::ortho(-OPENGL_WORLD_COORDINATE_X * WINDOW_ASPECT_RATIO, 
		       OPENGL_WORLD_COORDINATE_X  * WINDOW_ASPECT_RATIO,
		       -OPENGL_WORLD_COORDINATE_Y, OPENGL_WORLD_COORDINATE_Y, 
		       OPENGL_WORLD_NEAR, OPENGL_WORLD_FAR); // In world coordinates
	
	//p = glm::perspective(fieldOfView, aspectRatio, nearClippingPlane, farClippingPlane);
	
		//configure matrices - view matrix.
	setupCamera();
	v = glm::lookAt(cameraPosition, cameraTarget, upVector);

}

void view::setupCamera()
{ 
  cameraPosition =  glm::vec3(0, 0, 5/*addDist*1, -addDist*2*/); //same position as one lightsource. //same position as one lightsource./* glm::vec3(0, addDist*1, -addDist*2); //same position as one lightsource.*/
  cameraTarget = glm::vec3(0, 0, 0);
  upVector = glm::vec3(0, 1, 0);
  
}
