#include "QGLView.h"
#include <QtGui/qevent.h>

//#include <GL/gl.h>

#include <QSurfaceFormat>

#ifdef ENABLE_OPENCSG
#  include <opencsg.h>
#endif

#define DEBUG_SETVERTICES 0


OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
    , retinaScale(0.0f)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_device;
}
void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render()
{
    initializeOpenGLFunctions();

    if (!m_device)
      m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    changeSize();

    m_device->setSize(QSize(width, height));

    QPainter painter(m_device);
    render(&painter);
}

void OpenGLWindow::renderLater()
{
    requestUpdate();
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;

    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    changeSize();

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void OpenGLWindow::changeSize()
{
    initializeOpenGLFunctions();

    QSize size = this->size();

    if(width != size.width() || height != size.height())
    {
      retinaScale = devicePixelRatio();
      glViewport(0, 0, width * retinaScale, height * retinaScale);

      width  = size.width();
      height = size.height();
    }
}




static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void View::initialize()
{
/*
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram((QObject*)&oglw);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glClearColor(0.5, 0.0, 0.0, 1.0);

    //const qreal retinaScale = devicePixelRatio();
    //glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, -2);

    static GLfloat vertices[] = {
       -0.5f,  -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.5f,   0.5f, 0.0f
    };

    static GLfloat colors [] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    this->vertices = vertices;
    this->colors   = colors;

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, this->vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, this->colors);

    refreshRate = oglw.refreshRate();
*/
}

void View::render()
{
/*
    initializeOpenGLFunctions();

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    matrix.setToIdentity();

    matrix.rotate(100.0f * m_frame / refreshRate, 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
*/
}





View::View(QWidget *parent):
  QOpenGLWidget(parent),
  m_program(0),
  m_frame(0),
  vertices(nullptr),
  vertices_count(0)
{
  this->parent = parent;
  e_matrix = glm::mat4(1.0);
  this->mouse_drag_active = false;

  m_viewInstance = this;

   //seems not working all right: setSamles(4)- so anti-aliasing how to do that? //hw 2019-02-22
   QSurfaceFormat fmt;
   fmt.setVersion(2,0);
   //fmt.setDepthBufferSize(24);
   //fmt.setStencilBufferSize(8);
   //fmt.setProfile(QSurfaceFormat::NoProfile);
   //fmt.setRenderableType(QSurfaceFormat::RenderableType::OpenGLES);
   fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
   fmt.setSamples(4);
   this->setFormat(fmt);
   this->makeCurrent();
/*
   QSurfaceFormat fmt;
   fmt.setVersion(2,0);
   fmt.setDepthBufferSize(24);
   fmt.setStencilBufferSize(8);
   fmt.setProfile(QSurfaceFormat::NoProfile);
   fmt.setRenderableType(QSurfaceFormat::RenderableType::OpenGLES);
   fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
 
   //OpenGLWindow...
   this->oglw.setFormat(fmt);
   this->oglw.setAnimating(true);

   oglw.show();
*/
}
  
View::~View()
{
  
}

void View::initializeGL()
{
  GLView::initializeGL();

  initializeOpenGLFunctions();

  //oglw.initialize();

  this->init(); //old rotatingWheel
  this->initMatrices(); //...

 
/*
   m_context = new QOpenGLContext();
   m_context->setFormat(fmt);
   m_context->create();
   m_context->makeCurrent((QSurface*)this->parent->windowHandle());

  initializeOpenGLFunctions();
*/
//   wdgt->setFormat(fmt);
//   wdgt->makeCurrent();
   GLfloat far_far_away = 100.0f;
// 
//   InitOpenGLProgram();
//  
   glEnable(GL_DEPTH_TEST);
   glDepthRange(-far_far_away, +far_far_away);
// 
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// 
//   // The following line is reported to fix issue #71
   glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 64);
//   
//   IComponent cmp;
//   
//   Init(cmp);


     program.createShaders("simplevertshader.glsl", "simplefragshader.glsl");
     //program.printProgramLog();

}

  
void View::resizeGL(int w, int h)
{
  initializeOpenGLFunctions();

  glViewport(0, 0, w, h);
}


  
void View::paintGL()
{
  GLView::paintGL();

  static bool isInitialized = false;
  //oglw.render();
  //this->render();

  //wdgt->makeCurrent();
  
/*  QOpenGLContext *ctx = nullptr;
  ctx = this->context();
bool valid = ctx->isValid();
*/
    if(!isInitialized)
    {
      initializeOpenGLFunctions();
      isInitialized = true;
    }
     //program.createShaders("simplevertshader.glsl", "simplefragshader.glsl");
//  programID = program.getId();
//  assert(programID != -1);


//printf("Shaders loaded, and programID= %d\n", programID);

//glUseProgram(programID);	

  glUseProgram(program.getId());
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  
//  glClearColor(1.0, 0.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  //configure matrices - rotation matrix.
  myRotationAxis = glm::vec3(XAXIS, YAXIS, ZAXIS);
  rotMatrix = e_matrix * glm::rotate( e_matrix, angle, myRotationAxis );
  program.configure_mat4(UNILOC_ROTMATRIX, rotMatrix);

  // 	//configure matrices - view matrix.
  program.configure_mat4(UNILOC_V, this->getViewMatrix());


  program.configure_mat4(UNILOC_P, this->getProjectionMatrix());


  //configure matrices - inverse matrix for the v matrix.
  v_inv = glm::inverse(this->getViewMatrix());
  program.configure_mat4(UNILOC_V_INV, v_inv);

  mv_inv = glm::transpose(glm::inverse(this->getViewMatrix() * m));
  program.configure_mat4(UNILOC_MV_INV, mv_inv);

  //configure texture
  //program.configure_texture(UNILOC_MYTEXTURE2D, model.getTexture() /*mytexture2D*/, TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXTURE_TEXEL_SIZE);


  program.change_float(UNILOC_ALPHA, 1.0);

    static const GLfloat verts[] = {
       -0.5f,  -0.5f, 0.0f, //1.0f,
        0.25f,  -0.5f, 0.0f, //1.0f,
        0.5f,   0.5f, 0.0f,//, 1.0f
  //     -0.5f,  -0.5f, 0.0f, //1.0f,
//        0.5f,   0.5f, 0.0f,//, 1.0f
       -0.5f,   0.5f, 0.0f
    };

  // static const unsigned short indices[] = { 0, 1, 2, 0, 2, 3 };

    // const GLuint count_total_indices = 6;

  //configure arributes
  //if(vertices != nullptr)
    //program.configure_attributes(ATTRIB_VERTEX, &vertices[0], 3);
//  program.configure_attributes(ATTRIB_NORMAL, model.getNormalData(0),       3);

    //glDrawElements(GL_TRIANGLES,count_total_indices, GL_UNSIGNED_SHORT, (const GLvoid*)indices);

    std::vector<struct ObjectVertices*>::iterator it = objectNormals3d.begin();

    for(ObjectVertices* vtex : objectVertices3d)
    {
      struct ObjectVertices* nmal = *it++;
      //congfigure_attributes will enable vertex attribute array, but not disable it: we need to change that
      program.configure_attributes(ATTRIB_VERTEX, vtex->m_ptr, 4); //four GLfloats per vertex
      program.configure_attributes(ATTRIB_NORMAL, nmal->m_ptr, 3);


      //configure matrices - model matrix.
      scaleM = glm::scale(e_matrix, glm::vec3(scale, scale, scale)); 
      program.change_mat4(UNILOC_M , scaleM * vtex->m_m);

      glDrawArrays(GL_TRIANGLES, 0, vtex->m_size/4);

      GLint attrib_vtex = glGetAttribLocation(program.getId(), ATTRIB_VERTEX);
      GLint attrib_nmal = glGetAttribLocation(program.getId(), ATTRIB_NORMAL);

      glDisableVertexAttribArray(attrib_vtex);
      glDisableVertexAttribArray(attrib_nmal);
    }

    //glVertexAttribPointer(glGetAttribLocation(program.getId(),"vertex"), 3, GL_FLOAT, GL_FALSE, 0, &verts[0]);
    //glEnableVertexAttribArray(0);
  //if(vertices_count > 0 && vertices != nullptr)  
   // glDrawArrays(GL_TRIANGLES, 0, vertices_count);
    //glDisableVertexAttribArray(0);
}



void View::Init(IComponent cmp)
{
}

  
void View::ChangeWindowParameters(IComponent cmp)
{

}

  
void View::Render(IComponent cmp)
{

}

  
void View::RenderComponentGroup(IComponent cmp)
{

}

 
void View::RenderAllComponents()
{

}

  
void View::SwapBuffer()
{

}

 
void View::DrawToOfflineBuffer(IComponent cmp)
{

}

 
void View::SnapShotCurrentBuffer()
{

}

  
void View::SnapShotOfflineBuffer()
{

}


void View::ViewPreview()
{

}

  
void View::ViewAll()
{

}

  
void View::ViewZoomIn()
{

}

  
void View::ViewZoomOut()
{ 

  

}

  
void View::ResetView()
{

}

 
void View::ViewLeft()
{

}

  
void View::ViewRight()
{

}

  
void View::ViewTop()
{

}

  
void View::ViewBottom()
{

}

  
void View::ViewFront()
{

}

 
void View::ViewBack()
{

}

  
void View::ViewDiagonal()
{

}

  
void View::ViewCenter()
{

}

  
void View::ViewPerspective()
{

}

  
void View::ViewOrthogonal()
{

}

  
void View::ViewSurface()
{

}

  
void View::ViewWireframe()
{

}

 
void View::Teardown()
{

}

//used for setVertices


#define OSS_NOT_HERE(X) static_cast<std::ostringstream &&>(std::ostringstream() << X).str()

namespace { //used for setVertices
  std::string toString(const Vector3d &v)
{
	return OSS_NOT_HERE(v[0] << " " << v[1] << " " << v[2]);
}

Vector3d fromString(const std::string &vertexString) //used for setVertices
{
	Vector3d v;
	std::istringstream stream{vertexString};
	stream >> v[0] >> v[1] >> v[2];
	return v;
}

}


void View::clearVertices()
{
    objectVertices3d.clear();
    objectNormals3d.clear();
}

void View::setVertices(shared_ptr<const Geometry> root_geom)
{
  setVertices(root_geom, Transform3d::Identity(), false);
  
}

void View::setVertices(shared_ptr<const Geometry> root_geom, const Transform3d &m, bool clVerts)
{
  std::vector<GLfloat>* localVertices = new std::vector<GLfloat>();

  //clear old objects from view
  if(clVerts)
    clearVertices();
  
  objectVertices3d.reserve(MAX_ARRAY_SIZE_OPENGLES2);
  objectNormals3d.reserve(MAX_ARRAY_SIZE_OPENGLES2);
  
  PolySet ps(3);
  
	if (const CGAL_Nef_polyhedron *N = dynamic_cast<const CGAL_Nef_polyhedron *>(root_geom.get())) {
    if (!N->p3->is_simple())
      PRINT("WARNING: Exported object may not be a valid 2-manifold and may need repair : setVertices");
    
    if (!CGALUtils::createPolySetFromNefPolyhedron3(*(N->p3), ps))
      goto CONT_RENDER;
	}
	else if (const PolySet *pse = dynamic_cast<const PolySet *>(root_geom.get())) {
		ps = (PolySet)*pse;
    goto CONT_RENDER;
	}
	else if (dynamic_cast<const Polygon2d *>(root_geom.get())) {
		assert(false && "Unsupported dimension 2D");
	}
	else {
		PRINT("ERROR: Nef->PolySet failed in : setVertices");
    return;
	}
	
	
	
CONT_RENDER:	
	
  PolySet triangulated(3);
	PolysetUtils::tessellate_faces(ps, triangulated);
  if(DEBUG_SETVERTICES)
    std::cout << std::endl << std::endl << std::endl << "Start of setVertices output: " << std::endl;

	for(const auto &p : triangulated.polygons) {

	  assert(p.size() == 3); // STL only allows triangles
          
          localVertices->push_back((GLfloat)p[0](0,0));
          localVertices->push_back((GLfloat)p[0](1,0));
          localVertices->push_back((GLfloat)p[0](2,0));
          localVertices->push_back((GLfloat)1.0f); //every fourth value in a vertex will be 1.0f as vec4 is needed in shader.
    
          localVertices->push_back((GLfloat)p[1](0,0));
          localVertices->push_back((GLfloat)p[1](1,0));
          localVertices->push_back((GLfloat)p[1](2,0));
          localVertices->push_back((GLfloat)1.0f);
    
          localVertices->push_back((GLfloat)p[2](0,0));
          localVertices->push_back((GLfloat)p[2](1,0));
          localVertices->push_back((GLfloat)p[2](2,0));
          localVertices->push_back((GLfloat)1.0f);

          if(DEBUG_SETVERTICES)
          {
	    std::array<std::string, 3> vertexStrings;
	    std::transform(p.cbegin(), p.cend(), vertexStrings.begin(), toString);

	    Vector3d p0 = fromString(vertexStrings[0]);
	    Vector3d p1 = fromString(vertexStrings[1]);
	    Vector3d p2 = fromString(vertexStrings[2]);

                              /* precision           flags  coeffSep  rowSep rowPre  rowSuff matPre  matSuff */
            Eigen::IOFormat fmt(Eigen::StreamPrecision, 0,   "",      " ",     "",    "",      "",    "");

            std::stringstream ss;
            ss << p0.format(fmt) << std::endl;
            ss << p1.format(fmt) << std::endl;// << " ";
            ss << p2.format(fmt) << std::endl;// << " ";
            ss << p0.format(fmt) << std::endl << std::endl << std::endl;
            std::string s = ss.str();
            ss.str(s);
            std::cout << ss.str();// << std::endl << std::endl;
          }

          //split big objects into several pieces of the same size
          if(localVertices->size() >= MAX_ARRAY_SIZE_OPENGLES2)//objVertices->size >= MAX_ARRAY_SIZE_OPENGLES2)
          {
            int bigSize = localVertices->size();
            
            storeVertices(localVertices, m);
            storeNormals(localVertices);
            localVertices = new std::vector<GLfloat>();
          }
            
	}
  //if object is small enough - all vertices will be put in one array. Or: if objects has ben
  //split in loop above - save the rest of the vertices in the last array. 
  if(0 < localVertices->size() && localVertices->size() < MAX_ARRAY_SIZE_OPENGLES2)
  {
    int size = localVertices->size();

    storeVertices(localVertices, m);
    storeNormals(localVertices);
  }
}

//from original view class in rotatingWheel
void View::normalizedDeviceCoordinates(int screenX, int screenY, float scale,
				 float* normalizedX, float* normalizedY, glm::vec3* normalizedCursorPosition)
{
  
  *normalizedX =   (((float)screenX / WINDOW_WIDTH)  * 2.0 - 1.0) * OPENGL_WORLD_COORDINATE_X * WINDOW_ASPECT_RATIO / scale;
  
  *normalizedY = -(((float)screenY / WINDOW_HEIGHT) * 2.0 - 1.0) * OPENGL_WORLD_COORDINATE_Y / scale;
  
  *normalizedCursorPosition = glm::vec3(*normalizedX, *normalizedY, 0.0);

  std::cout << "normalizedCursorPosition: " << glm::to_string(*normalizedCursorPosition) << "\n" << "\n";
}

glm::mat4 View::getProjectionMatrix()
{
    return p;
}

glm::mat4 View::getViewMatrix()
{
    return v;
}

void View::init()
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
	
	glEnable(GL_MULTISAMPLE_ARB);
// 	glEnable(GL_DOUBLE);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glDepthFunc(GL_LESS); 
	glClearColor(0.8f, 0.8f, 0.6f, 1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport ( x, y, width, height ); //is working very well -> the object is inn one corner and smaller.  
} 

void View::initMatrices()
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

void View::setupCamera()
{ 
  cameraPosition =  glm::vec3(0, 0, 5/*addDist*1, -addDist*2*/); //same position as one lightsource. //same position as one lightsource./* glm::vec3(0, addDist*1, -addDist*2); //same position as one lightsource.*/
  cameraTarget = glm::vec3(0, 0, 0);
  upVector = glm::vec3(0, 1, 0);
  
}

//private functions
void View::InitOpenGLProgram()
{
//   QSurfaceFormat fmt;
//   fmt.setVersion(2,0);
//   fmt.setDepthBufferSize(24);
//   fmt.setStencilBufferSize(8);
//   fmt.setProfile(QSurfaceFormat::NoProfile);
//   fmt.setRenderableType(QSurfaceFormat::RenderableType::OpenGLES);
//   fmt.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
//   
//   m_context = new QOpenGLContext();
//   m_context->setFormat(fmt);
//   m_context->create();
//   this->setFormat(fmt);
//   this->makeCurrent();
  
//   wdgt->makeCurrent();
//   
//   initializeOpenGLFunctions();
// 
//   program.createShaders("simplevertshader.glsl", "simplefragshader.glsl");
//   programID = program.getId();
//   assert(programID != -1);
// 
//   printf("Shaders loaded, and programID= %d\n", programID);
// 
//   glUseProgram(programID);	


//   //configure matrices - rotation matrix.
//   myRotationAxis = glm::vec3(XAXIS, YAXIS, ZAXIS);
//   rotMatrix = e_matrix;/*glm::rotate( e_matrix, angle += 0.02f, myRotationAxis );*/
//   program.configure_mat4(UNILOC_ROTMATRIX, rotMatrix);
// 
//   //configure matrices - model matrix.
//   m = e_matrix;
//   program.configure_mat4(UNILOC_M, m);
// 
//   // 	//configure matrices - view matrix.
//   program.configure_mat4(UNILOC_V, view.getViewMatrix());
// 
// 
//   program.configure_mat4(UNILOC_P, view.getProjectionMatrix());
// 
// 
//   //configure matrices - inverse matrix for the v matrix.
//   v_inv = glm::inverse(view.getViewMatrix());
//   program.configure_mat4(UNILOC_V_INV, v_inv);
// 
//   mv_inv = glm::transpose(glm::inverse(view.getViewMatrix() * m));
//   program.configure_mat4(UNILOC_MV_INV, mv_inv);
// 
//   //configure texture
//   program.configure_texture(UNILOC_MYTEXTURE2D, model.getTexture() /*mytexture2D*/, TEXTURE_WIDTH * TEXTURE_HEIGHT * TEXTURE_TEXEL_SIZE);
// 
//   program.change_float(UNILOC_ALPHA, 1.0);
// 
//   //configure arributes
//   program.configure_attributes(ATTRIB_VERTEX, model.getVertexBufferData(0), NUM_ELEMENTS_PER_VERTEX_DATA);
//   program.configure_attributes(ATTRIB_NORMAL, model.getNormalData(0),       NUM_ELEMENTS_PER_NORMAL_DATA);
// 
//   program.printProgramLog();
}

/*
 * All functions downwards are in this file just to be able to integrate and test the new code above in the old file and class structure.
 * The functions downwards are not needed when the new class is ready for release.
 * 
 * 
 * 
 * 
 */

/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2011 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// #include "qtgettext.h"
// #include "QGLView.h"
// #include "Preferences.h"
// #include "renderer.h"
// 
// #include <QApplication>
// #include <QWheelEvent>
// #include <QCheckBox>
// #include <QDialogButtonBox>
// #include <QMouseEvent>
// #include <QMessageBox>
// #include <QPushButton>
// #include <QTimer>
// #include <QTextEdit>
// #include <QVBoxLayout>
// #include <QErrorMessage>
// #include "OpenCSGWarningDialog.h"
// #include "QSettingsCached.h"
// 
// 
// #include <stdio.h>
// #include <sstream>
// 
// #ifdef ENABLE_OPENCSG
// #  include <opencsg.h>
// #endif

// View::QGLView(QWidget *parent) :
// #ifdef USE_QOPENGLWIDGET
// 	QOpenGLWidget(parent)
// #else
// 	QGLWidget(parent)
// #endif
// {
// //   init();
// }

// #if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
// static bool running_under_wine = false;
// #endif

// void View::init()
// {
//   resetView();
// 
//   this->mouse_drag_active = false;
//   this->statusLabel = nullptr;
// 
//   setMouseTracking(true);
// 
// 
// 
// #if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
// // see paintGL() + issue160 + wine FAQ
// #include <windows.h>
//   HMODULE hntdll = GetModuleHandle(L"ntdll.dll");
//   if (hntdll)
//     if ( (void *)GetProcAddress(hntdll, "wine_get_version") )
//       running_under_wine = true;
// #endif
// }

// void View::resetView()
// {
// // 	cam.resetView();
// }

// void View::viewAll()
// {
// 	if (auto renderer = this->getRenderer()) {
// 		auto bbox = renderer->getBoundingBox();
// 		cam.object_trans = -bbox.center();
// 		cam.viewAll(renderer->getBoundingBox());
// 	}
// }

// void View::initializeGL()
// {
// //   auto err = glewInit();
// //   if (err != GLEW_OK) {
// //     fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
// //   }
// //   GLView::initializeGL();
// }

std::string View::getRendererInfo() const
{
  std::string fake = "No renderer info.\n";
//   info << glew_dump();
//   // Don't translate as translated text in the Library Info dialog is not wanted
// #ifdef USE_QOPENGLWIDGET
//   info << "\nQt graphics widget: QOpenGLWidget";
//   auto qsf = this->format();
//   auto rbits = qsf.redBufferSize();
//   auto gbits = qsf.greenBufferSize();
//   auto bbits = qsf.blueBufferSize();
//   auto abits = qsf.alphaBufferSize();
//   auto dbits = qsf.depthBufferSize();
//   auto sbits = qsf.stencilBufferSize();
//   info << boost::format("\nQSurfaceFormat: RGBA(%d%d%d%d), depth(%d), stencil(%d)\n\n") %
//     rbits % gbits % bbits % abits % dbits % sbits;
// #else
//   info << "\nQt graphics widget: QGLWidget";
// #endif
//   info << glew_extensions_dump();
  return fake;
}

#ifdef ENABLE_OPENCSG
void View::display_opencsg_warning()
{
// 	if (Preferences::inst()->getValue("advanced/opencsg_show_warning").toBool()) {
// 		QTimer::singleShot(0, this, SLOT(display_opencsg_warning_dialog()));
// 	}
}

void View::display_opencsg_warning_dialog()
{
// 	auto dialog = new OpenCSGWarningDialog(this);
// 
//   QString message;
//   if (this->is_opencsg_capable) {
//     message += _("Warning: You may experience OpenCSG rendering errors.\n\n");
//   }
//   else {
//     message += _("Warning: Missing OpenGL capabilities for OpenCSG - OpenCSG has been disabled.\n\n");
//     dialog->enableOpenCSGBox->hide();
//   }
//   message += _("It is highly recommended to use OpenSCAD on a system with "
// 							 "OpenGL 2.0 or later.\n"
// 							 "Your renderer information is as follows:\n");
//   QString rendererinfo(_("GLEW version %1\n%2 (%3)\nOpenGL version %4\n"));
//   message += rendererinfo.arg((const char *)glewGetString(GLEW_VERSION),
// 															(const char *)glGetString(GL_RENDERER),
// 															(const char *)glGetString(GL_VENDOR),
// 															(const char *)glGetString(GL_VERSION));
// 
//   dialog->setText(message);
//   dialog->enableOpenCSGBox->setChecked(Preferences::inst()->getValue("advanced/enable_opencsg_opengl1x").toBool());
//   dialog->exec();
// 
//   opencsg_support = this->is_opencsg_capable && Preferences::inst()->getValue("advanced/enable_opencsg_opengl1x").toBool();
}
#endif

// void View::resizeGL(int w, int h)
// {
// //   GLView::resizeGL(w,h);
// }

// void View::paintGL()
// {
//   GLView::paintGL();
// 
//   if (statusLabel) {
//     Camera nc(cam);
//     nc.gimbalDefaultTranslate();
// 		auto status = QString("%1 (%2x%3)")
// 			.arg(QString::fromStdString(nc.statusText()))
// 			.arg(size().rwidth())
// 			.arg(size().rheight());
//     statusLabel->setText(status);
//   }
// 
// #if defined(_WIN32) && !defined(USE_QOPENGLWIDGET)
//   if (running_under_wine) swapBuffers();
// #endif
// }

void View::mousePressEvent(QMouseEvent *event)
{
   mouse_drag_active = true;
   //last_mouse = event->globalPos();
}

void View::mouseDoubleClickEvent (QMouseEvent *event) {

// 	setupCamera();
// 
// 	int viewport[4];
// 	GLdouble modelview[16];
// 	GLdouble projection[16];
// 
// 	glGetIntegerv( GL_VIEWPORT, viewport);
// 	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
// 	glGetDoublev(GL_PROJECTION_MATRIX, projection);
// 
// 	double x = event->pos().x() * this->getDPI();
// 	double y = viewport[3] - event->pos().y() * this->getDPI();
// 	GLfloat z = 0;
// 
// 	glGetError(); // clear error state so we don't pick up previous errors
// 	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
// 	auto glError = glGetError();
// 	if (glError != GL_NO_ERROR) {
// 		return;
// 	}
// 
// 	if (z == 1) return; // outside object
// 
// 	GLdouble px, py, pz;
// 
// 	auto success = gluUnProject(x, y, z, modelview, projection, viewport, &px, &py, &pz);
// 
// 	if (success == GL_TRUE) {
// 		cam.object_trans -= Vector3d(px, py, pz);
// 		updateGL();
// 		emit doAnimateUpdate();
// 	}
}

void View::normalizeAngle(GLdouble& angle)
{
//   while(angle < 0) angle += 360;
//   while(angle > 360) angle -= 360;
}

void View::mouseMoveEvent(QMouseEvent *event)
{
//   auto this_mouse = event->globalPos();
//   double dx = (this_mouse.x() - last_mouse.x()) * 0.7;
//   double dy = (this_mouse.y() - last_mouse.y()) * 0.7;
     if (mouse_drag_active)
     if (event->buttons() & Qt::LeftButton
 #ifdef Q_OS_MAC
         && !(event->modifiers() & Qt::MetaModifier)
 #endif
        ) {
          angle += 0.2;

//       // Left button rotates in xz, Shift-left rotates in xy
//       // On Mac, Ctrl-Left is handled as right button on other platforms
//       cam.object_rot.x() += dy;
//       if ((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
//         cam.object_rot.y() += dx;
// 			}
//       else {
//         cam.object_rot.z() += dx;
// 			}
// 
//       normalizeAngle(cam.object_rot.x());
//       normalizeAngle(cam.object_rot.y());
//       normalizeAngle(cam.object_rot.z());
     } //else {
//       // Right button pans in the xz plane
//       // Middle button pans in the xy plane
//       // Shift-right and Shift-middle zooms
//       if ((QApplication::keyboardModifiers() & Qt::ShiftModifier) != 0) {
// 	      cam.zoom(-12.0 * dy);
//       } else {
// 
//       double mx = +(dx) * 3.0 * cam.zoomValue() / QWidget::width();
//       double mz = -(dy) * 3.0 * cam.zoomValue() / QWidget::height();
// 
//       double my = 0;
// #if (QT_VERSION < QT_VERSION_CHECK(4, 7, 0))
//       if (event->buttons() & Qt::MidButton) {
// #else
//       if (event->buttons() & Qt::MiddleButton) {
// #endif
//         my = mz;
//         mz = 0;
//         // actually lock the x-position
//         // (turns out to be easier to use than xy panning)
//         mx = 0;
//       }
// 
//       Matrix3d aax, aay, aaz;
//       aax = Eigen::AngleAxisd(-(cam.object_rot.x()/180) * M_PI, Vector3d::UnitX());
//       aay = Eigen::AngleAxisd(-(cam.object_rot.y()/180) * M_PI, Vector3d::UnitY());
//       aaz = Eigen::AngleAxisd(-(cam.object_rot.z()/180) * M_PI, Vector3d::UnitZ());
//       Matrix3d tm3 = Matrix3d::Identity();
//       tm3 = aaz * (aay * (aax * tm3));
// 
//       Matrix4d tm = Matrix4d::Identity();
//       for (int i=0;i<3;i++) for (int j=0;j<3;j++) tm(j,i) = tm3(j,i);
// 
//       Matrix4d vec;
//       vec <<
//         0,  0,  0,  mx,
//         0,  0,  0,  my,
//         0,  0,  0,  mz,
//         0,  0,  0,  1
//       ;
//       tm = tm * vec;
//       cam.object_trans.x() += tm(0,3);
//       cam.object_trans.y() += tm(1,3);
//       cam.object_trans.z() += tm(2,3);
//       }
//     }
     updateGL();
//     emit doAnimateUpdate();
//   }
     //last_mouse = this_mouse;
}

void View::mouseReleaseEvent(QMouseEvent*)
{
   mouse_drag_active = false;
   releaseMouse();
}

const QImage & View::grabFrame()
{
  const QImage *image = new  QImage();
// 	// Force reading from front buffer. Some configurations will read from the back buffer here.
// 	glReadBuffer(GL_FRONT);
// 	this->frame = grabFrameBuffer();
// 	return this->frame;
  return *image;
}

bool View::save(const char *filename)
{
//   return this->frame.save(filename, "PNG");
  return false;
}

void View::wheelEvent(QWheelEvent *event)
{
 #if QT_VERSION >= 0x050000
 	this->cam.zoom(event->angleDelta().y());
        scale += ((float) event->angleDelta().y())/1000.0;
        std::cout <<  scale << std::endl;
 #else
 	this->cam.zoom(event->delta());
        scale +=  ((float) event->angleDelta().y())/1000.0;
        std::cout << scale << std::endl;
 #endif
   updateGL();

  if(scale < 0.01 || 10.0 < scale)
    scale = 1.0;
}

// void View::ZoomIn(void)
// {
// //   this->cam.zoom(120);
// //   updateGL();
// }
// 
// void View::ZoomOut(void)
// {
// //   this->cam.zoom(-120);
// //   updateGL();
// }

void View::setOrthoMode(bool enabled)
{
// 	if (enabled) this->cam.setProjection(Camera::ProjectionType::ORTHOGONAL);
// 	else this->cam.setProjection(Camera::ProjectionType::PERSPECTIVE);
}


View* View::m_viewInstance = (View*)nullptr;
